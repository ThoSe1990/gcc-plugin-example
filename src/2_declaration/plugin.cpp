

#include "../gcc_includes.hpp"
#include "print_decl.hpp"

int plugin_is_GPL_compatible;

int plugin_init(struct plugin_name_args* plugin_info,
                struct plugin_gcc_version* version)
{
  register_callback(plugin_info->base_name, PLUGIN_FINISH_DECL,
                    declaration_callback, NULL);

  return 0;
}
