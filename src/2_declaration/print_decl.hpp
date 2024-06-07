#pragma once

#include "../gcc_includes.hpp"

static void print_integer(tree integer)
{
  switch (TYPE_PRECISION(integer))
  {
    case 8:
      std::cout << "char";
      break;
    case 16:
      std::cout << "short";
      break;
    case 32:
      std::cout << "int";
      break;
    case 64:
      std::cout << "long long";
      break;
    default:
      std::cout << "unknown integer type with precision: "
                << TYPE_PRECISION(integer);
  }
}

static void print_floating_point(tree floating_point)
{
  if (TYPE_PRECISION(floating_point) == 32)
  {
    std::cout << "float";
  }
  else if (TYPE_PRECISION(floating_point) == 64)
  {
    std::cout << "double";
  }
  else
  {
    std::cout << "Floating point type with precision "
              << TYPE_PRECISION(floating_point);
  }
}
static void print_type(tree node);
static void print_function_arguments(tree type)
{
  if (TREE_CODE(type) == FUNCTION_TYPE)
  {
    std::cout << " args: (";
    for (tree arg = TYPE_ARG_TYPES(type); arg; arg = TREE_CHAIN(arg))
    {
      print_type(TREE_VALUE(arg));
      if (TREE_CHAIN(arg))
      {
        std::cout << ", ";
      }
    }
    std::cout << ")";
  }
}
static void print_qualifiers(tree node)
{
  if (TYPE_QUALS(node) & TYPE_QUAL_CONST)
  {
    std::cout << "const ";
  }
  // other qualifiers ...
}
static void print_type(tree type)
{
  print_qualifiers(type);

  switch (TREE_CODE(type))
  {
    case INTEGER_TYPE:
    {
      if (TYPE_UNSIGNED(type))
      {
        std::cout << "unsigned ";
      }
      print_integer(type);
    }
    break;
    case REAL_TYPE:
      print_floating_point(type);
      break;
    case POINTER_TYPE:
      print_type(TREE_TYPE(type));
      std::cout << '*';
      break;
    case ARRAY_TYPE:
      print_type(TREE_TYPE(type));
      std::cout << "[]";
      break;
    case RECORD_TYPE:  // classes and structs
      if (TYPE_NAME(type) && TREE_CODE(TYPE_NAME(type)) == TYPE_DECL)
      {
        std::cout << IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(type)));
      }
      else
      {
        std::cout << "unnamed struct or class";
      }
      break;
    case UNION_TYPE:
      if (TYPE_NAME(type) && TREE_CODE(TYPE_NAME(type)) == TYPE_DECL)
      {
        std::cout << IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(type)));
      }
      else
      {
        std::cout << "unnamed union";
      }
      break;
    case VOID_TYPE:
      std::cout << "void";
      break;
    case FUNCTION_TYPE:
      print_type(TREE_TYPE(type));
      print_function_arguments(type);
      break;
    case REFERENCE_TYPE:
      print_type(TREE_TYPE(type));
      std::cout << '&';
      break;
    default:
      std::cout << "unknown type: " << get_tree_code_name(TREE_CODE(type));
      break;
  }
  std::cout << ' ';
}

static void print_name(tree node)
{
  tree name = DECL_NAME(node);
  if (!name)
  {
    return;
  }
  std::cout << ' ' << IDENTIFIER_POINTER(name);
}

static void print_name_with_scope(tree node)
{
  std::string str;

  while (node && DECL_NAME(node) && TREE_CODE(node) != TRANSLATION_UNIT_DECL)
  {
    str = "::" + std::string(IDENTIFIER_POINTER(DECL_NAME(node))) + str;
    node = DECL_CONTEXT(node);
  }
  std::cout << str;
}

static void print_unsigned(tree node)
{
  if (TYPE_UNSIGNED(node))
  {
    std::cout << "unsigned ";
  }
}

void print_value(tree var)
{
  if (!var)
  {
    std::cout << " uninitialized";
    return;
  }
  std::cout << ' ';
  switch (TREE_CODE(var))
  {
    case INTEGER_CST:
      std::cout << TREE_INT_CST_LOW(var);
      break;
    case REAL_CST:
      char buffer[64];
      real_to_decimal(buffer, &TREE_REAL_CST(var), sizeof(buffer),
                      0, 1);
      std::cout << buffer;
      break;
    case STRING_CST:
    case ADDR_EXPR:  // check NOP_EXPR there we go through ADDR_EXPR
    case COMPOUND_LITERAL_EXPR:
    case ARRAY_REF:
      std::cout << __FILE__ << ':' << __LINE__
                << "  TREE CODE: " << TREE_CODE(var)
                << " not implemented";
      break;
    case OBJ_TYPE_REF:
      std::cout << " object type reference, not implemented yet";
      break;
    case CONSTRUCTOR:  // == arrays
      for (std::size_t i = 0; i < CONSTRUCTOR_NELTS(var); ++i)
      {
        if (i == 0)
        {
          std::cout << "{ ";
        }
        if (i > 0)
        {
          std::cout << ", ";
        }
        std::cout << '[' << i << ']';
        print_value(CONSTRUCTOR_ELT(var, i)->value);
        if (i == CONSTRUCTOR_NELTS(var) - 1)
        {
          std::cout << " }";
        }
      }
      break;
    case NON_LVALUE_EXPR:
      print_value(TREE_OPERAND(var, 0));
      break;
    case NOP_EXPR:
    {
      // TODO recursive call ...
      tree operand = TREE_OPERAND(var, 0);
      if (TREE_CODE(operand) == ADDR_EXPR)
      {
        tree inner_operand = TREE_OPERAND(operand, 0);
        if (TREE_CODE(inner_operand) == STRING_CST)
        {
          std::cout << TREE_STRING_POINTER(inner_operand);
        }
        else if (TREE_CODE(inner_operand) == VAR_DECL)
        {
          std::cout << " reference to: ";
          print_type(inner_operand);
          print_name(inner_operand);
          print_value(DECL_INITIAL(inner_operand));
        }
        else
        {
          std::cout << " with unknown address expression, TREE_CODE: "
                    << TREE_CODE(inner_operand);
        }
      }
      else
      {
        std::cout << " with unknown NOP expression";
      }
      break;
    }
    case TARGET_EXPR:
    case INIT_EXPR:
    case AGGR_INIT_EXPR:
      std::cout << "*_EXPR";
      break;
    case DEFERRED_PARSE:
      std::cout << " deferred parse expression";
      break;
    default:
      std::cout << "implement me, TREE_CODE = " << TREE_CODE(var)
                << ", " << get_tree_code_name(TREE_CODE(var));
  }
}

static void print_declaration(tree node)
{
   print_type(TREE_TYPE(node));
  print_name_with_scope(node);
  // print_name(node);
  print_value(DECL_INITIAL(node)); 
  std::cout << '\n';
}

static void declaration_callback(void* gcc_data, void* user_data)
{
  tree node = (tree)gcc_data;
  if (TREE_CODE_CLASS(TREE_CODE(node)) != tcc_declaration)
  {
    std::cerr << "Tree node (" << TREE_CODE(node) << ") '" << get_tree_code_name(TREE_CODE(node)) << "' is no declaration.\n";
    return;
  }
  print_type(TREE_TYPE(node));
  print_name_with_scope(node);
  // print_name(node);
  print_value(DECL_INITIAL(node));
  std::cout << '\n';
}