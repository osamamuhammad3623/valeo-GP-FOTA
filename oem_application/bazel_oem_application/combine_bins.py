#define ROOT_CERT_ADDRESS 				 APPLICATION_EXENSIONS_ADDRESS
#define SB_CERT_ADDRESS 			    (APPLICATION_EXENSIONS_ADDRESS + 0x0800)
#define META_DATA_ADDRESS               (APPLICATION_EXENSIONS_ADDRESS + 0x01000)
#define SIGNATURE_ADDRESS               (APPLICATION_EXENSIONS_ADDRESS + 0x01400)
#define CERTIFICATES_METADATA_ADDRESS   (APPLICATION_EXENSIONS_ADDRESS + 0x01602)


def combine_security_files(img_num):
    # Open file for writing in binary mode
    with open(f"combined_{img_num}.bin", "wb") as f1:
        # Seek to the desired offset (e.g. 100 bytes from the beginning of the file)
        with open("ROOT__cert.bin", "rb") as f2:
            data=f2.read()
            f1.write(data)
        with open("SB_cert.bin", "rb") as f2:
            data=f2.read()
            f1.seek(0x0800)
            f1.write(data)
        with open(f"metadata_{img_num}.txt", "rb") as f2:
            data=f2.read()
            f1.seek(0x01000)
            f1.write(data)
        with open("signature.bin", "rb") as f2:
            data=f2.read()
            f1.seek(0x01400)
            f1.write(data)
        with open("certificates_metadata.bin", "rb") as f2:
            data=f2.read()
            f1.seek(0x01602)
            f1.write(data)