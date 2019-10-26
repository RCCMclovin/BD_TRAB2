record_file = open("entrada.csv","r")
output_file = open("record_file.txt","w")
lines = record_file.readlines()
for line in lines:
    aux = line.replace('"','').split(";")
    for field in aux:
        if len(field) > 0:
            output_file.write(field+"\n")
record_file.close()
output_file.close()