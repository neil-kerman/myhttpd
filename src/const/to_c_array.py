import sys
import re
import os

resource_filename = sys.argv[1]
print(resource_filename)
output_dir = sys.argv[2]
print(output_dir)
filename_in_c = os.path.basename(resource_filename).replace(".", "_")
output_filename = output_dir + "/" + filename_in_c + ".c"
resource_file = open(resource_filename, "rb")
output_file = open(output_filename, "w")

output_file.write("const char " + filename_in_c + "[] = {")
for byte in bytearray(resource_file.read()):
    output_file.write("0x{:X},".format(byte))
resource_file.close()
output_file.write("};" + "\r\n")
output_file.write("const unsigned long long " + filename_in_c + "_size = sizeof(" + filename_in_c + ");")
output_file.close()
