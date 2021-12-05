

from rak811.rak811_v3 import Rak811


lora = Rak811()

# Most of the setup should happen only once...
print('Setup')

response = lora.set_config('lora:work_mode:1')
for r in response:
    print(r)
mode_text = lora.set_config('lorap2p:transfer_mode:1')
#for r in mode_text:

# RF configuration
# - Avoid LoRaWan channels (You will get quite a lot of spurious packets!)
# - Respect local regulation (frequency, power, duty cycle)
freq = 869.800
sf = 12
bw = 0  # 125KHz
ci = 3  # 4/5
pre = 8
pwr = 17
lora.set_config(f'lorap2p:{int(freq*1000*1000)}:{sf}:{bw}:{ci}:{pre}:{pwr}')
#lora.send_p2p(b'helloworld')
print('ok')
while True:
    lora.receive_p2p(1000)
    while lora.nb_downlinks > 0:
        message = lora.get_downlink()
        data = message['data'].decode('UTF-8').errors('ignore')
        print(data)