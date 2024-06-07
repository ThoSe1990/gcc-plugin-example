
#include "../gcc_includes.hpp"
#include <iostream>

int plugin_is_GPL_compatible;

// Function to process GIMPLE statements
static void process_gimple_stmt(gimple_stmt_iterator* gsi)
{
  gimple* stmt = gsi_stmt(*gsi);
  print_gimple_stmt(stdout, stmt, 0, TDF_SLIM);
  std::cout << std::endl;
}

// Function to process a GIMPLE function
static void process_gimple_function(function* fun)
{
  basic_block bb;
  FOR_EACH_BB_FN(bb, fun)
  {
    gimple_stmt_iterator gsi;
    for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
    {
      process_gimple_stmt(&gsi);
    }
  }
}

// Example of a pass to process GIMPLE
const pass_data my_pass_data = {
    GIMPLE_PASS,   /* type */
    "my_pass",     /* name */
    OPTGROUP_NONE, /* optinfo_flags */
    TV_NONE,       /* tv_id */
    0,             /* properties_required */
    0,             /* properties_provided */
    0,             /* properties_destroyed */
    0,             /* todo_flags_start */
    0              /* todo_flags_finish */
};

class my_gimple_pass : public gimple_opt_pass
{
 public:
  my_gimple_pass(gcc::context* ctxt) : gimple_opt_pass(my_pass_data, ctxt) {}

  unsigned int execute(function* fun) override
  {
    std::cout << "Processing function: " << function_name(fun) << std::endl;
    process_gimple_function(fun);
    return 0;
  }
};

// Plugin initialization function
int plugin_init(struct plugin_name_args* plugin_info,
                struct plugin_gcc_version* version)
{
  struct register_pass_info pass_info;

  pass_info.pass = new my_gimple_pass(g);
  pass_info.reference_pass_name = "cfg";
  pass_info.ref_pass_instance_number = 1;
  pass_info.pos_op = PASS_POS_INSERT_AFTER;

  register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL,
                    &pass_info);

  return 0;
}
