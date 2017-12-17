# Geometrijski algoritmi @ MATF 

Projekat za predmet **Geometrijski algoritmi** na master studijama na Matematičkom fakultetu. 

Profesor: dr Predrag Janičić [www.matf.bg.ac.rs/~janicic]\
Asistent: Aleksandra Kocić [www.matf.bg.ac.rs/~aleksandra_kocic]

Studenti:
* 

## Uputstvo za studente ##

### Inicijalna podešavanja ###

* Instalirati Qt i git
* Napraviti nalog na bitbucket-u
* Upisati nalog u tabelu: ```https://docs.google.com/spreadsheets/d/17a4MRa9qIeUMygo92MOwqKnVV9_ELblfVLKhSmJhY0c/edit#gid=0 ``` (bez obzira što još uvek nemate izabrane projekte, upišite ime, prezime i korisničko ime)
* Napraviti folder na lokalnom računaru, pozicionirati se u folder i klonirati repozirotijum komandom ```git clone https://bitbucket.org/ga1718team/algorithms.git ```

### Unit testovi ### 
 Prezentacija sa časa: http://poincare.matf.bg.ac.rs/~asimic/2017/ga/7/UnitTests.pdf \
 Na kraju prezentacije su date sve informacije o integraciji unit testova sa nasim projektom. \

### Link do šablona za projektnu dokumentaciju ###
https://docs.google.com/document/d/10zCuGxrLbyux7fsQ6qOlZkl4rErQndfw06WiUanXnGs/edit?usp=sharing

### Struktura direktorijuma ###

  * algorithms_practice [Sadrži primere sa časova vežbi]
    * ga00_alg1.cpp
    * ga00_alg1.h
    * ...
    * ...
    * gaXY_algXY.cpp
    * gaXY_algXY.h 
  * algorithms_projects [Sadrži studentske projekte]
    * ga00_emptyProject.cpp
    * ga00_emptyProject.h
    * ga01_temaPrvogStudentaIzTabele.cpp
    * ga01_temaPrvogStudentaIzTabele.h
    * ga02_temaDrugogStudentaIzTabele.cpp
    * ga02_temaDrugogStudentaIzTabele.h
    * ...
    * ...   
  * docs [Sadrži projektnu dokumentaciju]
    * ga00_convexHull.docx
    * ga00_convexHull.pdf
    * ga01_temaPrvogStudenta.docx
    * ga01_temaPrvogStudenta.pdf
    *...
    *...
    * ga25_temaPoslednjegStudenta.docx
    * ga25_temaPoslednjegStudenta.pdf
  * input_files [Sadrži foldere sa raznim unapred zadatim ulazima za Vaše algoritme] 
    * ga01_temaPrvogStudenta
    *...
    * ga25_temaPoslednjegStudenta 

### Osnovne komande
* **git pull** - da prevučete trenutni kod 
* **git add \<fajlovi koje hocete da dodate>** 
* **git commit -m 'Poruka koja opisuje to sto se odradili'**
* **git push origin master** - da posaljete svoje izmene na server

#### ***Svaki put kada nešto radite na projektu, odradite pull komandu pre nego što počnete (kako biste uvek radili sa najnovijom verzijom koda)*** ####