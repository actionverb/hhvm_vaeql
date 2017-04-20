#include <sys/types.h>
#include <sys/stat.h>

#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/base/array-init.h"
#include "VaeQueryLanguageLexer.h"
#include "VaeQueryLanguageParser.h"
#include "VaeQueryLanguageTreeParser.h"

#include "vaeql.h"

#define EMPTY_STRING ""

namespace HPHP {

const StaticString
  s_first("0"),
  s_second("1");

static Variant HHVM_FUNCTION(_vaeql_query_internal, const Variant& arg) {
  int64_t int_val;
  String arr_val[2];
  
  /* PHP */
  String query;
  
  query = "";
  int_val = 0;
  arr_val[0] = arr_val[1] = "";
  if (arg.isString()) {
    query = arg.toString();
  }

  /* VaeQueryLanguage */
  VaeQueryLanguageParser_start_return langAST;
  pVaeQueryLanguageLexer	lxr;
  pVaeQueryLanguageParser psr;
  pVaeQueryLanguageTreeParser treePsr;
  pANTLR3_INPUT_STREAM istream;
  pANTLR3_COMMON_TOKEN_STREAM	tstream;
  pANTLR3_COMMON_TREE_NODE_STREAM	nodes;
  VaeQueryLanguageTreeParser_start_return result;
  
  /* Lex and Parse */
  if (istream = antlr3NewAsciiStringInPlaceStream((uint8_t *)query.c_str(), (ANTLR3_UINT64)query.length(), NULL)) {
    if (lxr	= VaeQueryLanguageLexerNew(istream)) {
      if (tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr))) {
        if (psr	= VaeQueryLanguageParserNew(tstream)) {
          langAST = psr->start(psr);
          if (psr->pParser->rec->state->errorCount == 0) {
            if (nodes = antlr3CommonTreeNodeStreamNewTree(langAST.tree, ANTLR3_SIZE_HINT)) {
              if (treePsr = VaeQueryLanguageTreeParserNew(nodes)) {
                result = treePsr->start(treePsr);
                if (result.result) {
//                  arr_val[0] = String(result.isPath);
//		  arr_val[1] = String((const char *)result.result->chars);

Array ret;
ret.set(s_first, String(result.isPath));
ret.set(s_second, String((const char *)result.result->chars));
/*
SAFEARRAY * pSafeArray;
SAFEARRAYBOUND aDim[1];
aDim[0].lLbound = 0;
aDim[0].cElements = 2;
variant.vt = VT_ARRAY | VT_BSTR;
pSafeArray = SafeArrayCreate(VT_BSTR, 1, aDim);
BSTR* dwArray = NULL;
SafeArrayAccessData(pSafeArray, (void**)&dwArray);
dwArray[0] = String(result.isPath).c_str();
dwArray[1] = String((const char *)result.result->chars).c_str();
SafeArrayUnaccessData(pSafeArray);
variant.parray = pSafeArray;

                  return variant; 
*/
return ret;
                } else {
                  int_val = -2;
                }
                treePsr->free(treePsr);
              } else {
                int_val = -101;
              } 
              nodes->free(nodes);
            } else {
              int_val = -102;
            }
          } else {
            int_val = -1;
          }
          psr->free(psr);
        } else {
          int_val = -103;
        }
        tstream->free(tstream);
      } else {
        int_val = -104;
      }
      lxr->free(lxr);
    } else {
      int_val = -105;
    }
    istream->close(istream);
  } else {
    int_val = -106;
  }
    
  return Variant(int_val);
}

static class vaeqlExtension : public Extension {
  public:
    vaeqlExtension() : Extension("vaeql", "1.0.1-dev") {}
    virtual void moduleInit() {
      HHVM_FE(_vaeql_query_internal);
      loadSystemlib();
    }
} s_vaeql_extension;

HHVM_GET_MODULE(vaeql)

} // namespace HPHP
