#include <sys/types.h>
#include <sys/stat.h>

#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/base/hphp-system.h"
#include "hphp/runtime/base/array-init.h"
#include "hphp/runtime/vm/func.h"
#include "hphp/runtime/vm/runtime.h"
#include "hphp/runtime/vm/jit/translator.h"
#include "hphp/runtime/vm/jit/translator-inline.h"
#include "hphp/system/systemlib.h"

#include "VaeQueryLanguageLexer.h"
#include "VaeQueryLanguageParser.h"
#include "VaeQueryLanguageTreeParser.h"

#include "vaeql.h"


char * resolveFunction(char * function, char ** args) {
  HPHP::Array params = HPHP::Array::Create();
  params.append(function);
  HPHP::Array ar_args = HPHP::Array::Create();
  for (char ** arg = args; *arg; arg++) {
    ar_args.append(*arg);
  }
  params.append(ar_args);
  auto res = HPHP::vm_call_user_func("_vaeql_function", HPHP::Variant{params}, false);
  char * ret = new char[res.toString().length()];
  strcpy(ret, res.toString().c_str());
printf("resolveFunction() = %s\n", ret); 
  return ret;
}

RangeFunctionRange resolveRangeFunction(char * function, char ** args) {
  RangeFunctionRange r;
  r.low = 0;
  r.high = 0;
  HPHP::Array params = HPHP::Array::Create();
  params.append(function);
  HPHP::Array ar_args = HPHP::Array::Create();
  for (char ** arg = args; *arg; arg++) {
    ar_args.append(*arg);
  }
  params.append(ar_args);
  auto res = HPHP::vm_call_user_func("_vaeql_range_function", HPHP::Variant{params}, false);
  if (!res.isArray()) {
    return r;
  }
  HPHP::Array ret_val = res.toArray();
  if (!ret_val.size()) {
    return r;
  }
  r.high = 99999999999999;
  if (!ret_val.lvalAt(0).isNull()) {
    auto r0 = ret_val.lvalAt(0);
    if (r0.isInteger()) {
      r.low = r0.toInt64Val();
    }
  }
  if (!ret_val.lvalAt(1).isNull()) {
    auto r1 = ret_val.lvalAt(1);
    if (r1.isInteger()) {
      r.high = r1.toInt64Val();
    }
  }
printf("range -> %ld-%ld\n", r.low, r.high);
  return r;
}

char * resolvePath(char * variable) {
  auto res = HPHP::vm_call_user_func("_vaeql_path", HPHP::Variant{HPHP::Array::Create(variable)}, false);
  char * ret = new char[res.toString().length()];
  strcpy(ret, res.toString().c_str());
printf("resolvePath(%s) = %s\n", variable, ret); 
  return ret;
}

char * resolveVariable(char * variable) {
  auto res = HPHP::vm_call_user_func("_vaeql_variable", HPHP::Variant{HPHP::Array::Create(variable)}, false);
  char * ret = new char[res.toString().length()];
  strcpy(ret, res.toString().c_str());
printf("resolveVariable(%s) = %s\n", variable, ret); 
  return ret;
}

namespace HPHP {

const StaticString
  s_first("0"),
  s_second("1");

static Variant HHVM_FUNCTION(_vaeql_query_internal, const Variant& arg) {
  VMRegAnchor _;
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
  istream = antlr3NewAsciiStringInPlaceStream((uint8_t *)query.c_str(), (ANTLR3_UINT64)query.length(), NULL);
  if (istream != NULL) {
    lxr = VaeQueryLanguageLexerNew(istream);
    if (lxr != NULL) {
      tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));
      if (tstream != NULL) {
        psr = VaeQueryLanguageParserNew(tstream);
        if (psr != NULL) {
          langAST = psr->start(psr);
          if (psr->pParser->rec->state->errorCount == 0) {
            nodes = antlr3CommonTreeNodeStreamNewTree(langAST.tree, ANTLR3_SIZE_HINT);
            if (nodes != NULL) {
              treePsr = VaeQueryLanguageTreeParserNew(nodes);
              if (treePsr != NULL) {
                result = treePsr->start(treePsr);
                if (result.result) {
                  Array ret;
                  ret.set(s_first, String(result.isPath));
                  ret.set(s_second, String((const char *)result.result->chars));
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
