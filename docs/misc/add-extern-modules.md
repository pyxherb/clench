# Add External Modules

To add external modules, you may specify
`-DCLENCH_EXTERN_MODULE_NAME_LIST=<external module name list>` in the CMake
configuration command or set it in the CMake cache manually.

Don't forget to add corresponding module path variable entry, the name is
`CLENCH_EXTERN_MODULE_PATH_<module name>`, the path should be absolute.

Note that you cannot external dynamic modules while you are using static
linking.
