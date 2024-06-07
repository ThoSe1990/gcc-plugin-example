#include "../gcc_includes.hpp"
#include "../2_declaration/print_decl.hpp"

int plugin_is_GPL_compatible;

static void traverse_tree(tree node);

static void print_call_expression(tree node)
{
  tree call_expr_fn = CALL_EXPR_FN(node);
  tree func_decl = TREE_OPERAND(call_expr_fn, 0);
  std::cout << "-- call: '" << IDENTIFIER_POINTER(DECL_NAME(func_decl))
            << "'\n";

  const std::size_t argc = call_expr_nargs(node);
  std::cout << "-- arg count: " << argc << '\n';
  for (int i = 0; i < call_expr_nargs(node); ++i)
  {
    traverse_tree(CALL_EXPR_ARG(node, i));
  }

  tree return_type = TREE_TYPE(TREE_TYPE(func_decl));
  std::cout << "-- returns: " << get_tree_code_name(TREE_CODE(return_type))
            << '\n';
}

static void expression(tree node)
{
  switch (TREE_CODE(node))
  {
    case EXPR_STMT:
      std::cout << "-- accessing node in expression statement\n";
      traverse_tree(TREE_OPERAND(node, 0));
      break;
    case CLEANUP_POINT_EXPR:
      std::cout << "-- accessing node in clean up point\n";
      traverse_tree(TREE_OPERAND(node, 0));
      break;
    case INIT_EXPR:
    {
      tree lhs = TREE_OPERAND(node, 0);
      tree rhs = TREE_OPERAND(node, 1);

      std::cout << "-- intializing variable lhs = "
                << get_tree_code_name(TREE_CODE(lhs))
                << ", rhs = " << get_tree_code_name(TREE_CODE(rhs)) << '\n';

      traverse_tree(lhs);
      traverse_tree(rhs);
    }
    break;
    case CALL_EXPR:
      print_call_expression(node);
      break;
    default:
      std::cout << "Unhandled expression: '"
                << get_tree_code_name(TREE_CODE(node)) << "'\n";
      break;
  }
}
static void unary(tree node)
{
  switch (TREE_CODE(node))
  {
    case CONVERT_EXPR:
      std::cout << "-- accessing tree in convert expression\n";
      traverse_tree(TREE_OPERAND(node, 0));
      break;
    default:
      std::cout << "Unhandled unary: '" << get_tree_code_name(TREE_CODE(node))
                << "'\n";
      break;
  }
}
static void statement(tree node)
{
  std::cout << "statement " << get_tree_code_name(TREE_CODE(node)) << '\n';
}
static void exceptional(tree node)
{
  switch (TREE_CODE(node))
  {
    case STATEMENT_LIST:
    {
      std::cout << "-- iterating over statement list\n";
      for (tree_stmt_iterator tsi = tsi_start(node); !tsi_end_p(tsi);
           tsi_next(&tsi))
      {
        traverse_tree(tsi_stmt(tsi));
      }
    }
    break;
    default:
      break;
  }
}
static void declaration(tree node)
{
  switch (TREE_CODE(node))
  {
    case FUNCTION_DECL:
    {
      std::cout << "-- traversing through function tree\n";
      traverse_tree(DECL_SAVED_TREE(node));
    }
    break;
    case VAR_DECL:
      // TODO check print_declaration(..)
      print_name_with_scope(node);
      std::cout << '\n';
      break;
    default:
      std::cout << "Unhandled declaration: '"
                << get_tree_code_name(TREE_CODE(node)) << "'\n";
      break;
  }
}

static void constant(tree node)
{
  switch (TREE_CODE(node))
  {
    case VOID_CST:
      std::cout << "-- void constant, we're done now\n";
      break;
    case INTEGER_CST:
      std::cout << "-- integer constant: " << TREE_INT_CST_LOW(node) << '\n';
      break;
    default:
      std::cout << "Unhandled declaration: '"
                << get_tree_code_name(TREE_CODE(node)) << "'\n";
      break;
  }
}

static void traverse_tree(tree node)
{
  switch (TREE_CODE_CLASS(TREE_CODE(node)))
  {
    case tcc_statement:
      statement(node);
      break;
    case tcc_expression:
      expression(node);
      break;
    case tcc_declaration:
      declaration(node);
      break;
    case tcc_exceptional:
      exceptional(node);
      break;
    case tcc_unary:
      unary(node);
      break;
    case tcc_constant:
      constant(node);
      break;
    case tcc_vl_exp:
      expression(node);
      break;
    default:
      std::cout << "Unhandled tree code class: ("
                << TREE_CODE_CLASS(TREE_CODE(node)) << ") '"
                << get_tree_code_name(TREE_CODE(node)) << "'\n";
      break;
  }
}

static void pre_genericize_callback(void* gcc_data, void* user_data)
{
  tree node = (tree)gcc_data;
  std::cout << "-- pre genericize callback: current node = '"
            << get_tree_code_name(TREE_CODE(node)) << "'\n";
  traverse_tree(node);
}

int plugin_init(struct plugin_name_args* plugin_info,
                struct plugin_gcc_version* version)
{
  register_callback(plugin_info->base_name, PLUGIN_FINISH_DECL,
                    declaration_callback, NULL);
  register_callback(plugin_info->base_name, PLUGIN_PRE_GENERICIZE,
                    pre_genericize_callback, NULL);
  return 0;
}
