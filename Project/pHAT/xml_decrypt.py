
# encrypted messagge (in HEX):640B05147943303A65607696A1CE4DC288
# decrypted messagge (in HEX):543A32353A553A313536363A583A313131 
# this is the original messagge: T:25:U:1566:X:111

from lora.payload import LoRaPayload

xmlstr = '''<?xml version="1.0" encoding="UTF-8"?>
<DevEUI_uplink xmlns="http://uri.actility.com/lora">abc1234a
<FCntUp>2</FCntUp>14
<payload_hex>543A32353A553A313536363A583A316131</payload_hex>[...]
</DevEUI_uplink>'''

payload = LoRaPayload(xmlstr)

key = 'AABBCCDDEEFFAABBCCDDEEFFAABBCCDD'
dev_addr = 'abc1234a'
plaintext = payload.decrypt(key, dev_addr)

plaintext_hex = "".join(str(x) for x in plaintext)

print(plaintext_hex)