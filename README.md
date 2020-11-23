# Αρχιτεκτονική Υπολογιστών - Εργασία 1 
# Βουλγαράκης Ρωμανός | Μουστάκας Βασίλειος - Παναγιώτης
# 9383 | 9424

Αρχικά εκτελείται η εξής εντολή:

```sh
$ cd  ./build/ARM/gem5.opt -d hello_result configs/example/arm/starter_se.py--cpu="minor" "tests/test-progs/hello/bin/arm/linux/hello"
```
# Ερώτηση 1 
Μέσα στο starter_se.py εντοπίζονται τα εξής χαρατηριστικά:
* CPU type: MinorCPU
* System clock: 1GHz
* Voltage 3.3V  
* L1 (instruction & data) cache
* L2 cache
* Memory mode :	timing
* 2 memory controllers
* Memory size: 2147483648 bytes = 2 GB

# Ερώτηση 2
Τα παραπάνω επαληθεύονται από τις κάτωθι γραμμές:
* sim_freq  1000000000000 # Frequency of simulated ticks
* system.voltage_domain.voltage 3.300000  # Voltage in Volts
* mem_mode: "timing"
* mem_ranges: "0:2147483648"
* stats.txt: system.cpu_cluster.cpus.committedInsts           5028                       # Number of instructions committed
* system.cpu_cluster.l2.overall_accesses::total          479                       # number of overall (read+write) accesses

Τα L2 cache accesses θα μπορούσαν θεωρητικά να υπολογιστούν από τα L1 cache (instruction + data) misses. Παρατηρείται όμως ότι αν και αυτό επαληθεύεται για την icache, δεν ισχύει το ίδιο για την dcache.  

# Ερώτηση 3

Σύντομη περιγραφή των μοντέλων επεξεργαστή του gem5:

To SimpleCPU είναι ένα καθαρά λειτουργικό in-order μοντέλο το οποίο χωρίζεται σε 3 κλάσεις, την BaseSimpleCPU, την AtomicSimpleCPU και την TimingSimpleCPU. Η BaseSimpleCPU δεν μπορεί να χρησιμοποιηθεί από μόνη της, σε αντίθεση με τις άλλες δύο τις οποίες κληρονομεί. Η βασική διαφορά μεταξύ των AtomicSimpleCPU και TimingSimpleCPU είναι ο τύπος πρόσβασης μνήμης που χρησιμοποιούν. Η πρώτη χρησιμοποιεί atomic memory accesses, ενώ η δεύτερη timing memory accesses.

Το O3CPU είναι ένα out-of-order μοντέλο. Σε αντίθεση με τα περισσότερα μοντέλα, τα οποία εκτελούν τις εντολές είτε στην αρχή είτε στο τέλος του pipeline, το συγκεκριμένο εκτελεί τις εντολές στην αρχή του pipeline και στην συνέχεια τις περνάει σε ένα timing backend.

Το μοντέλο Trace CPU στοχεύει στην μελέτη της απόδοσης της μνήμης του συστήματος με ακρίβεια και ταχύτερα σε σχέση  με τον μοντέλο O3CPU.  Αυτό επιτυγχάνεται με την χρήση ελαστικών ιχνών, τα οποία παράγονται από το Elastic Trace Probe που βρίσκεται στο μοντέλο O3CPU.

Το MinorCPU είναι ένα in-order μοντέλο με σταθερό pipeline αλλά με διαμορφώσιμες δομές δεδομένων και συμπεριφοράς κατά την εκτέλεση. Στόχος του είναι η παροχή ενός framework για την μίκρο-αρχιτεκτονική συσχέτιση του μοντέλου με συγκεκριμένους επεξεργαστές παρόμοιων δυνατοτήτων.

Πηγή: https://www.gem5.org/documentation/general_docs/cpu_models/SimpleCPU

Επιλέχθηκε το εξής πρόγραμμα στη C με όνομα simple_c.c .
```sh
#include <stdio.h>
int main() {
    int num;
    num = 13;
    // True if num is perfectly divisible by 2
    if(num % 2 == 0)
        printf("%d is even.", num);
    else
        printf("%d is odd.", num);
    
    return 0;
}

```

To compilation σε ARM έγινε με την εξής εντολή:
```sh
arm-linux-gnueabihf-gcc --static simple_c.c -o simple_c_arm
```
Στη συνέχεια το binary εκτελέστηκε με την εξής εντολή:
```sh
./build/ARM/gem5.opt -d c_program_minor_standard configs/example/se.py --cpu-type="MinorCPU" --caches --cmd=tests/test-progs/hello/bin/arm/linux/simple_c_arm
```
Χρησιμοποιήθηκε το script "se.py" με τύπο CPU: "Minor"

Χρόνος εκτέλεσης στον simulated επεξεργαστή: 0.000033 s.

Χρησιμοποιώντας "TimingSimpleCPU":
```sh
./build/ARM/gem5.opt -d c_program_timing_standard configs/example/se.py --cpu-type="TimingSimpleCPU" --caches --cmd=tests/test-progs/hello/bin/arm/linux/simple_c_arm
```
Χρόνος εκτέλεσης στον simulated επεξεργαστή: 0.000038 s.

Η διαφορά στον χρόνο εκτέλεσης είναι σχετικά μικρή, καθώς και τα δύο μοντέλα είναι αναλυτικά.

Με MinorCPU και clock = 10MHz:
```sh
./build/ARM/gem5.opt -d c_program_minor_clock configs/example/se.py --sys-clock=10Mhz --cpu-type="MinorCPU" --caches --cmd=tests/test-progs/hello/bin/arm/linux/simple_c_arm
```
Χρόνος εκτέλεσης στον simulated επεξεργαστή: 0.000631 s.

Με TimingSimpleCPU και clock = 10MHz:
```sh
./build/ARM/gem5.opt -d c_program_timing_clock configs/example/se.py --sys-clock=10Mhz --cpu-type="TimingSimpleCPU" --caches --cmd=tests/test-progs/hello/bin/arm/linux/simple_c_arm
```
Χρόνος εκτέλεσης στον simulated επεξεργαστή: 0.000636 s.

Βλέπουμε πολύ μεγαλύτερο χρόνο εκτέλεσης, πράγμα λογικό αφού μειώθηκε η συχνότητα λειτουργίας των επεξεργαστών.

Με MinorCPU, clock = 1GHz και τύπο μνήμης DDR4_2400_8x8: 
```sh
./build/ARM/gem5.opt -d c_program_minor_mem configs/example/se.py --mem-type=DDR4_2400_8x8 --cpu-type="MinorCPU" --caches --cmd=tests/test-progs/hello/bin/arm/linux/simple_c_arm
```
Χρόνος εκτέλεσης στον simulated επεξεργαστή: 0.000032 s.

Με TimingSimpleCPU, clock = 1GHz και τύπο μνήμης DDR4_2400_8x8: 
```sh
./build/ARM/gem5.opt -d c_program_timing_mem configs/example/se.py --mem-type=DDR4_2400_8x8 --cpu-type="TimingSimpleCPU" --caches --cmd=tests/test-progs/hello/bin/arm/linux/simple_c_arm
```
Χρόνος εκτέλεσης στον simulated επεξεργαστή: 0.000037 s.

Μειώθηκε οριακά ο χρόνος εκτέλεσης και στους δύο επεξεργαστές, πράγμα λογικό καθώς η καινούρια μνήμη είναι ελαφρώς γρηγορότερη.
