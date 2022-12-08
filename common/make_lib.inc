!ifdef MAKE_LIB_DEFINED
!error MAKE_LIB already included
!endif

PROJECT_TYPE=lib

!include $(ROOT_DIR)\common\make.inc

RTL_DIR=$(ROOT_DIR)\RTL
RTL_HEADERS=$(RTL_DIR)\include
RTL_LIBS=$(RTL_DIR)\lib

{.\}.h{$(RTL_HEADERS)}.h:
        $(COPY_SOURCE_SILENT)

{.\}.hpp{$(RTL_HEADERS)}.hpp:
        $(COPY_SOURCE_SILENT)

PH=$(RTL_HEADERS)\name

PUBLISHED_LIB=$(RTL_LIBS)\$(PROJECT_NAME).$(PROJECT_TYPE)

$(RTL_LIBS) $(RTL_HEADERS):
       $(ENSURE_TARGET_DIR)

$(PUBLISHED_LIB):: $(RTL_LIBS)

$(PUBLISHED_LIB):: $(APP_OUTPUT_FILE)
        $(COPY_SOURCE_SILENT)
        
MAKE_LIB_DEFINED=
