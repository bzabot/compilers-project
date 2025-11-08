#include <stdio.h>
#include "parser.h"
#include "printAST.c"

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
  } //  yyin = stdin
  if (yyparse() == 0)
  {
    if (program_root != 0)
    {
      printf("PROCEDURE: %s\n", program_root->procedure_name);
      printf("BEGIN\n");
      if (program_root->body != 0)
      {
        printCmd(program_root->body, 1);
      }
      printf("\nEND %s\n", program_root->procedure_name);
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
