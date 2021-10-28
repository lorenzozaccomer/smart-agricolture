

import codecs

hex = "404A23C1AB0085010174F2F25959595D2BAC0E5D7F8B4DCFF7463930682D"
b64 = codecs.encode(codecs.decode(hex, 'hex'), 'base64').decode()

print(b64)