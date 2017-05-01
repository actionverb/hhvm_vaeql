HHVM_EXTENSION(vaeql ext_vaeql.cpp VaeQueryLanguageLexer.c VaeQueryLanguageParser.c VaeQueryLanguageTreeParser.c)
HHVM_SYSTEMLIB(vaeql ext_vaeql.php)
target_link_libraries(vaeql antlr3c)
