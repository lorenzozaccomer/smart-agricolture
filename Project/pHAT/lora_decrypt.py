

# encrypted messagge (in HEX):74F2F25959595D2BAC0E5D7F8B4DCFF746 
# decrypted messagge (in HEX):543A32353A553A313536363A583A313131 
# this is the original messagge: T:25:U:1566:X:111


#devAddr = abc1234a
#nwkSKey = 238792b74801de223b50ddd309b8f139
#appSKey = 744826b8703e23a2e6cb8d79eed484bb

from lora.crypto import loramac_decrypt

#payload = '543A32353A553A313536363A583A313131'
payload = '404A23C1AB0085010174F2F25959595D2BAC0E5D7F8B4DCFF7463930682D'
sequence_counter = 185
key = '744826b8703e23a2e6cb8d79eed484bb'
dev_addr = 'abc1234a'

plaintext_ints = loramac_decrypt(payload, sequence_counter, key, dev_addr)
print(plaintext_ints)

plaintext_hex = "".join(str(x) for x in plaintext_ints)

print(plaintext_hex)