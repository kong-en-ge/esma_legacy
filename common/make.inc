OUTPUT_DIR=outward
APP_OUTPUT_NAME=$(OUTPUT_DIR)\$(PROJECT_NAME)

ENSURE_DIR=IF NOT EXIST $@ md $@

.prj.mak:
  prj2mak $< $@ $(@R)

.mak{$(OUTPUT_DIR)}.lib:
  $(MAKE) -f$** $@

$(OUTPUT_DIR):
  $(ENSURE_DIR)

update: $(OUTPUT_DIR)

$(PROJECT_NAME).prj: update

project: $(APP_OUTPUT_NAME).$(PROJECT_TYPE)
