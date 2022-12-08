OUTPUT_DIR=outward
APP_OUTPUT_NAME=$(OUTPUT_DIR)\$(PROJECT_NAME)
APP_OUTPUT_FILE=$(APP_OUTPUT_NAME).$(PROJECT_TYPE)

ENSURE_DIR=IF NOT EXIST name MD name
ENSURE_TARGET_DIR=$(ENSURE_DIR:name=$@)
COPY_SOURCE_SILENT=copy $** $@ >nul

null_target:
  rem

.prj.mak:
  prj2mak $< $@ $(@R)

.mak{$(OUTPUT_DIR)}.lib:
  $(MAKE) -f$** $@

.mak{$(OUTPUT_DIR)}.exe:
  $(MAKE) -f$** $@

GET_LIB=$(MAKE) -f$(ROOT_DIR)\common\get_lib.mak LIB_NAME=name ROOT_DIR=$(ROOT_DIR)
GET_HEADERS=$(MAKE) -f$(ROOT_DIR)\common\get_lib.mak LIB_NAME=name ROOT_DIR=$(ROOT_DIR) headers

$(OUTPUT_DIR):
  $(ENSURE_TARGET_DIR)
  ATTRIB +H $@

update:: $(OUTPUT_DIR)

$(PROJECT_NAME).prj: update

project: $(APP_OUTPUT_FILE)

release: project
!if ("$(PROJECT_TYPE)"=="EXE") || ("$(PROJECT_TYPE)"=="exe")
  tdstrip $(APP_OUTPUT_FILE)
!endif

