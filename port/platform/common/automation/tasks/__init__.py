from invoke import Collection, Config
from scripts import u_utils
from . import nrfconnect, nrf5, stm32cube, esp_idf, arduino, automation, linux

ns = Collection()

cfg = Config()
cfg['root_dir'] = u_utils.UBXLIB_DIR
cfg['vscode_dir'] = u_utils.UBXLIB_DIR + '/.vscode'
cfg['cfg_dir'] = u_utils.AUTOMATION_DIR

ns.configure(cfg)
ns.add_collection(nrfconnect)
ns.add_collection(nrf5)
ns.add_collection(stm32cube)
ns.add_collection(esp_idf)
ns.add_collection(arduino)
ns.add_collection(automation)
ns.add_collection(linux)
