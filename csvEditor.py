import csv

file = open("./student-mat.csv")
reader = csv.reader(file, delimiter=';')
formattedCSV = open("student-mat-formatted.csv", 'w')
wr = csv.writer(formattedCSV, quoting=csv.QUOTE_ALL)

i = 1
for row in reader:
    if(row[0] == "GP"):
        row[0] = 1
    elif(row[0] == "MS"):
        row[0] = 0
    wr.writerow(row)
        

