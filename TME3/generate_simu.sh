make
./simulator -m 0 -M 15 -s 1 -e 100 -K 32 -N 128 -D "rep-hard" -o simulations/sim1.csv
./simulator -m 0 -M 12 -s 1 -e 100 -K 32 -N 128 -D "rep-soft" -o simulations/sim2.csv
./simulator -m 0 -M 12 -s 1 -e 100 -K 32 -N  96 -D "rep-soft" -o simulations/sim3.csv
./simulator -m 0 -M 12 -s 1 -e 100 -K 32 -N  64 -D "rep-soft" -o simulations/sim4.csv
./simulator -m 0 -M 12 -s 1 -e 100 -K 32 -N  32 -D "rep-soft" -o simulations/sim5.csv