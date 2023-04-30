import os, hashlib, binascii


'''
A function to get the size of a file
'''
def get_file_size(file_path):
    return os.path.getsize(file_path)


'''
A function to calculate the digest of a file
'''
def get_file_digest(file_path):
    with open(file_path, 'rb', buffering=0) as f:
        return hashlib.file_digest(f, 'sha256').digest()


'''
A function to calculate the CRC of a file
'''
def get_file_crc(file_path):
    buf = open(file_path,'rb').read()
    hash = binascii.crc32(buf) & 0xFFFFFFFF
    return int("%08X" % hash, 16)
