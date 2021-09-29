

from rak811.rak811_v3 import Rak811


lora = Rak811()

# Most of the setup should happen only once...
print('Setup')

response = lora.set_config('lora:work_mode:1')
for r in response:
    print(r)
mode_text = lora.set_config('lorap2p:transfer_mode:2')
#for r in mode_text:

# RF configuration
# - Avoid LoRaWan channels (You will get quite a lot of spurious packets!)
# - Respect local regulation (frequency, power, duty cycle)
freq = 869.800
sf = 7
bw = 0  # 125KHz
ci = 3  # 4/5
pre = 8
pwr = 16
lora.set_config(f'lorap2p:{int(freq*1000*1000)}:{sf}:{bw}:{ci}:{pre}:{pwr}')
lora.send_p2p(b'helloworld')
print('ok')
# 
# print('Entering send/receive loop')
# counter = 0
# try:
#     while True:
#         # Calculate next message send timestamp
#         next_send = time() + P2P_BASE + randint(0, P2P_RANDOM)
#         # Set module in receive mode
#         lora.set_config('lorap2p:transfer_mode:1')
#         # Loop until we reach the next send time
#         # Don't enter loop for small wait times (<1 1 sec.)
#         while (time() + 1) < next_send:
#             wait_time = next_send - time()
#             print('Waiting on message for {:0.0f} seconds'.format(wait_time))
#             # Note that you don't have to listen actively for capturing message
#             # Once in receive mode, the library will capture all messages sent.
#             lora.receive_p2p(wait_time)
#             while lora.nb_downlinks > 0:
#                 message = lora.get_downlink()
#                 data = message['data']
#                 if data[:len(P2P_MAGIC)] == P2P_MAGIC:
#                     print(
#                         'Received message: {}'.format(
#                             int.from_bytes(data[len(P2P_MAGIC):],
#                                            byteorder='big')
#                         )
#                     )
#                     print('RSSI: {}, SNR: {}'.format(message['rssi'],
#                                                      message['snr']))
#                 else:
#                     print('Foreign message received')
#         # Time to send message
#         counter += 1
#         print('Send message {}'.format(counter))
#         # Set module in send mode
#         lora.set_config('lorap2p:transfer_mode:2')
#         lora.send_p2p(P2P_MAGIC + bytes.fromhex('{:08x}'.format(counter)))
# 
# except:  # noqa: E722
#     pass
# 
# print('All done')
# exit(0)