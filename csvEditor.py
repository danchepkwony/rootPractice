import csv

file = open("./student-mat.csv")
reader = csv.reader(file, delimiter=';')
signalCSV = open("student-mat-signal.csv", 'w')
backgroundCSV = open("student-mat-background.csv", 'w') 
wr = csv.writer(signalCSV, quoting=csv.QUOTE_ALL)
wr2 = csv.writer(backgroundCSV, quoting=csv.QUOTE_ALL)

i = 1
for row in reader:
    if(i == 1):
        wr.writerow(row)
        wr2.writerow(row)
    i += 1
    if(row[len(row)-1] != "G3"):
        if(int(row[len(row)-1]) >= 10):
            wr.writerow(row)
        if(int(row[len(row)-1]) < 10):
            wr2.writerow(row)

