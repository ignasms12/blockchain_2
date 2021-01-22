## 2-oji užduotis: Supaprastintos blokų grandinės (blockchain) kūrimas

Šiame release pateiktas v0.2 blockchain versija.

Programos paleidimui naudokite make (daryta ant macOS - dependency path'ai gali skirtis):

* make - šios komandos pagalba sukompiliuosite pateiktą kodą
* make run - šios komandos pagalba paleisite programą

### Features'ai:
* Generuojami 5 blokai, žiūrima kurio iš jų hashinimas vyksta greičiausiai (ne pagal laiką, pagal iteration'ų kiekį), renkamasis pirmasis kuriame užtrunka mažiau nei 100 000 kartų (arba 200 000, 300 000 ir t.t.)
* Pridėti check'ai ar index'as nebuvo anksčiau naudotas (dedant transakcijas į vieną bloką ir pasirenkant vieną iš penkių blokų)
