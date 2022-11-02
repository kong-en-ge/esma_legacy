OUTPUT_DIR=outward
APP_OUTPUT_NAME=$(OUTPUT_DIR)\$(PROJECT_NAME)

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

project: $(APP_OUTPUT_NAME).$(PROJECT_TYPE)
