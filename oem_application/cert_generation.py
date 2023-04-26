from OpenSSL import crypto, SSL

def generate_self_signed_cerificate():

    #can look at the generated file using openssl:
    #openssl x509 -inform pem -in selfsigned.crt -noout -text

    # create a key pair
    k = crypto.PKey()
    k.generate_key(crypto.TYPE_RSA, 4096)

    # create a self-signed cert
    cert = crypto.X509()
    cert.get_subject().C = "EG"
    cert.get_subject().ST = "Cairo"
    cert.get_subject().L = "Cairo"
    cert.get_subject().O = "Valeo FOTA GP"
    cert.get_subject().OU = "Valeo FOTA GP"
    cert.get_subject().CN = "AAA"
    cert.get_subject().emailAddress = "someone@gmail.com"
    cert.set_serial_number(0)
    cert.gmtime_adj_notBefore(0)
    cert.gmtime_adj_notAfter(10*365*24*60*60) # 10 years
    cert.set_issuer(cert.get_subject())
    cert.set_pubkey(k)
    cert.sign(k, 'sha256') # sign using sha256
    with open("self_signed.crt", "wt") as f:
        f.write(crypto.dump_certificate(crypto.FILETYPE_PEM, cert).decode("utf-8"))
    with open("private.key", "wt") as f:
        f.write(crypto.dump_privatekey(crypto.FILETYPE_PEM, k).decode("utf-8"))
