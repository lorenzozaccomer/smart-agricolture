# from rak811.rak811 import Mode, Rak811
# 
# lora = Rak811()
# lora.hard_reset()
# lora.mode = Mode.LoRaWan
# lora.band = 'EU868'
# lora.set_config(dev_eui='60C5A8FFFE7986A0',
# app_eui='AC1F09FFF8680811',
# app_key='A6CA67AE17CFB3ODA999189F13642156')
# print("testo di prova")
# lora.join_otaa()
# lora.dr = 5
# lora.send('Hello world')
# lora.close()

#!/usr/bin/env python3
# V3.0.x firmware
from rak811.rak811_v3 import Rak811

lora = Rak811()
lora.set_config('lora:work_mode:0')
lora.set_config('lora:join_mode:0')
lora.set_config('lora:region:EU868')
lora.set_config('lora:app_eui:60C5A8FFFE7986A0')
lora.set_config('lora:app_key:7808088192B3B0653DB92C662589578D')
lora.join()
lora.set_config('lora:dr:5')
lora.send('Hello world')
lora.close()