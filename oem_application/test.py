import hashlib

f = open("test_app.bin", 'r', buffering=0)
original_bin_hash = hashlib.file_digest(f, 'sha256').digest()

hash = bytes.fromhex("f2c4a08f2bd5d7df5440b7eb3f85f6694ee13a4de6e25d20c45c56f12b0cfbc0")

if original_bin_hash == hash:
    print("yes")
    metadata_file = open('test.txt', 'w')
    metadata_file.write(str(original_bin_hash))


# result = hashlib.sha256(hash.encode()).hexdigest()
# print(len(result))