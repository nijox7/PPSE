import csv
import matplotlib.pyplot as plt

x=[]
y=[]

with open('simulations/sim4.csv') as csv_file:
    csv_reader = csv.DictReader(csv_file)
    line_count = 0
    for row in csv_reader:
        if line_count == 0:
            print(f'Column names are {", ".join(row)}')
            line_count += 1
        else:
            x.append(float(row['Eb/N0']))
            y.append(float(row['FER']))
            line_count += 1
    print(f'Processed {line_count} lines.')
plt.plot(x,y, marker='o')
plt.title('Title')
plt.xlabel('SNR')
plt.ylabel('BER')

plt.show()    
