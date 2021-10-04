
record_file = open("entrada.csv","r", encoding="utf8", errors="ignore")
output_file = open("record_file.txt","w")
lines = record_file.readlines()
i = 0
for line in lines:
    if i ==20:
        break
    aux = line.split('";"')
    #print(aux)
    for field in aux:
        if len(field) > 0:
            output_file.write(field.replace('"','')+"\n")
        else:
            output_file.write("*\n")
    i+=1
record_file.close()
output_file.close()
