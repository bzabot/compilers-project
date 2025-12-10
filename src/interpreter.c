#include <stdio.h>
#include "parser.h"
#include "printAST.c"
#include "semantics.h"
#include "codegen.h"

int main(int argc, char **argv)
{
  --argc;
  ++argv;
  if (argc != 0)
  {
    yyin = fopen(*argv, "r");
    if (!yyin)
    {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } 
  if (yyparse() == 0)
  {
    if (program_root != 0)
    {
      if (check_semantics(program_root) != 0) {
          fprintf(stderr, "Compilation failed due to semantic errors.\n");
          return 1;
      }

      generate_code(program_root);
    }
    else if (root != 0)
    {
      printAbsTree(root, 0);
      printf("\n");
    }
    else if (root2 != 0)
    {
      printBoolAbsTree(root2, 0);
      printf("\n");
    }
    else if (cmd_root != 0)
    {
      printCmd(cmd_root, 0);
      printf("\n");
    }
  }
  return 0;
}
