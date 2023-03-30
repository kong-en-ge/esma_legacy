PROJECT_TYPE=exe

!include $(ROOT_DIR)\common\make.inc

common_libs:
        $(GET_LIB:name=corelib)
        $(GET_LIB:name=graphics)
        $(GET_LIB:name=dbf)
        $(GET_LIB:name=files)
        $(GET_LIB:name=servis)

update:: common_libs

run: $(APP_OUTPUT_FILE)
  $(APP_OUTPUT_FILE) /workpath:data

