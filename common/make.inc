OUTPUT_DIR=outward
APP_OUTPUT_NAME=$(OUTPUT_DIR)\$(PROJECT_NAME)
APP_OUTPUT_FILE=$(APP_OUTPUT_NAME).$(PROJECT_TYPE)

ENSURE_DIR=IF NOT EXIST $@ md $@

.prj.mak:
  prj2mak $< $@ $(@R)

.mak{$(OUTPUT_DIR)}.lib:
  $(MAKE) -f$** $@

.mak{$(OUTPUT_DIR)}.exe:
  $(MAKE) -f$** $@

$(OUTPUT_DIR):
  $(ENSURE_DIR)
  ATTRIB +H $@

update:: $(OUTPUT_DIR)

$(PROJECT_NAME).prj: update

project: $(APP_OUTPUT_FILE)

release: project
!if ("$(PROJECT_TYPE)"=="EXE") || ("$(PROJECT_TYPE)"=="exe")
  tdstrip $(APP_OUTPUT_FILE)
!endif

