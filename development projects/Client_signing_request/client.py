import requests
import base64

url = "https://signing-server.onrender.com"

#get request
# res = requests.get(url)
# print(res.json())

#post request
binary_data = open("metadata.txt","rb").read()
base64_data = base64.b64encode(binary_data).decode('utf-8')
url2 = url+"/sign"
post_data ={"name": "metadata.txt", "content":base64_data, "password": "123456789"}
response = requests.post(url2, json=post_data)

print(response.status_code)
data =response.json()
if(response.status_code ==200):
    Root_der_cert_base64 = data["ROOT_DER_CERT"]
    SB_der_cert_base64 = data["SB_DER_CERT"]
    signature_base64 = data["SIGNATURE"]
    Root_der_cert_bin = base64.b64decode(Root_der_cert_base64)
    SB_der_cert_bin = base64.b64decode(SB_der_cert_base64)
    signature_bin = base64.b64decode(signature_base64)

    with open('ROOT__cert.bin', 'wb') as file:
        file.write(Root_der_cert_bin)
    with open('SB_cert.bin', 'wb') as file:
        file.write(SB_der_cert_bin)
    with open('signature.bin', 'wb') as file:
        file.write(signature_bin)
else:
    print(data["detail"])

