# Ohjelmointi 3 Hajoitustyö, ryhmä ´609uu´

## Yleiskuvaus

Toteutimme harjoitustyössä pelin, jossa pelaaja syntyy Tampereen keskustaan, missä hänellä on hetki aikaa yrittää ampua
mahdollisimman monta linja-autoa ja matkustajaa. Pelistä oli ideana lähteä tekemään jonkinlaista bullet-hell tyyppistä
yksinpeliä, mutta rajallisten resurssien vuoksi ihan niin pitkälle ei päästy.  Lopputuotetta voisi luonnehtia aikaiseksi
kehitysvaiheeksi, missä on toteutettuna vasta pistelaskuri, pelaajan liike ja ampuminen. Periaatteessa peli vaatisi enää
jonkinlaisen vihollisen toteuttamista, minkä jälkeen voisi jo sanoa yhden tason olevan toteutettuna. Vihollisia voisi
toteuttaa erilaisina IActor:sta periytettyinä Actoreina. Voisi olla järkevää tehdä myös jonkinlainen BaseEnemy
-tyyppinen ratkaisu, josta voitaisiin luoda helpommin uusia vihollisia, joita voidaan käsitellä logiikassa keskenään
samalla tavalla.

Pelin käynnistyessä avautuu ensin aloitusdialogi. Tässä voi asettaa pelaajalle nopeuden sekä pelille aikarajan. Näitä
kenttiä voitaisiin käyttää esimerkiksi siihen, että haetaan sopivia arvoja pelin lopulliselle versiolle. Aloitusdialogi
on määritelty MainWindow -luokassa.

Kun aloitusdialogissa painetaan aloitusnappia, varsinainen pelinäkymä eli GameWidget asetetaan näkyviin ikkunaan.
Samalla MainWindow emittoi signalin, mistä Engine tietää käynnistää pelilogiikan. Engine käynnistää QTimerin, jonka
jokaisella timeoutilla Kaupungin tila etenee yhdellä "askeleella". Tämän lisäksi Engine kutsuu Logic::finalizeGameStart
funktiota käynnistäen CourseLib:n logiikan, joka käsittelee linja-autoja ja matkustajia.

Engine kuuntelee erinäisiä signaleja, joiden kautta pelin eri palikat pystyvät kommunikoimaan keskenään. Engine toimii
pelimoottorin tapaan eräänlaisena keskusyksikkönä, minkä kautta kaikki kommunikointi tapahtuu. Näin pystytään välttymään
suunnattomilta osoitinviidakoilta ja samalla ohjelman laajennettavuus paranee. Engineen pystytään periaatteessa
kytkemään ihan mitä tahansa olioita kiinni ja määrittelemään Enginessä olioiden interaktiot. Esimerkiksi tilastoja
kerätään Statistics oliolla, jonka funktioita kutsutaan Enginessä.

Kun edellä mainittu QTimer on ollut käynnissä aloitusdialogissa määritellyn ajan, Engine pysäyttää pelilogiikan ja
informoi siitä MainWindowia. Tällöin MainWindow vaihtaa näkyviin lopetusikkunan, missä näytetään kyseisen pelikerran
tilastot. Pisteitä pelaaja saa siitä, paljonko nyssejä ja matkustajia se saa tuhottua. Tämän lisäksi näytetään
osumatarkkuus ym. nice-to-know tietoja.

## Dokumentaatio

Tässä dokumentissa esitellään yleistason kuvaus ohjelman tärkeimmistä luokista, niiden vastuunjaosta ja ohjelman
kokonaisarkkitehtuurista. Tarkempi kuvaus, mukaanlukien esimerkiksi rajapinnat, sopimussuunnittelut yms., on
kommentoituna ohjelman header-tiedostoihin. Doxygen-ohjelmalla voi generoida niistä automaattisesti siistin
dokumentaation. Documentation-kansiosta löytyy kaksi tiedostoa: Doxyfile-Nysse ja Doxyfile-Game. Ensiksi mainittu luo
dokumentaation koko projektista mukaan lukien CourseLib. Jälkimmäinen luo dokumentaation vain projektin itse
toteutetuista osista, eli Game-aliprojektista. Dokumentaatiot gneroidaan ./doxyfiles/nysse/out ja ./doxyfiles/game/out
-hakemistoihin.

Yksikkötestejä ei ole liiemmin dokumentoitu, sillä ne ovat erittäin yksinkertaisia, mikä johtuu siitä että Statistics
luokkakin jäi melko yksinkertaiseksi. Muille luokille yksikkötestejä ei ehditty, mutta esimerkiksi Math -luokalla olisi
hyvä varmistaa laskuoperaatioiden toimivuus.

.so rakenne.md

## Lisäominaisuudet
Toteutimme seuraavat lisäominaisuudet:
- Tasainen ruudunpäivitys
	- Engine kutsuu kaupungin ja pelaajana advance() metodit Engine::framerateTgt -muuttujan mukaisella taajuudella.
	Kyseiset metodit taas johtavat ruudun päivittämiseen, minkä takia ruudun päivitystaajuus on Engine::framerateTgt (niissä
	määrin kun tietokone jaksaa laskea).
- Vieritettävä kartta
	- Pelaajan liikuttaminen emittoi signaalin, jonka perusteella MainWindow ja GameWidget keskittävät pelinäkymän pelaajan
	sijaintiin. Qt käsittelee tilanteet, missä pelaajan sijaintiin ei voida tarkalleen keskittää näkymää (knu pelaaja on
	kartan kulmassa). City huolehtii siitä, että pelaaja ei pääse poistumaan pelialueelta.
- Pelihahmon tasainen liike
	- Engine::kboardHandler liikuttaa Pelaajaa jokaisessa pelin askeleessa (Enginen::timer::timeout) näppäinten määräämään
	suuntaan näppäinpainallusten muodostaman yksikkovektorin ja pelaajan liikenopeuden tulon verran
- Pelin tilan seuranta
	- Statistics pitää reaaliaikaisesti kirjaa tilastoista. Engine::statsTimer::timeout on kytketty enginen slottiin, joka
	kysyy Statistics:lta tilastoja ja käskee MainWindowia päivittämään niitä vastaavat tekstikentät.
- Oma lisäominaisuus: tuki N käyttöliittymälle MainWindow:ssa + GameWidget
	- MainWindow:ssa on mahdollisuus näyttää montaa erilaista käyttöliittymää avaamatta uusia pop-up ikkunoita tai
	menettämättä näkymien tietoja eri näkymien välillä liikuttaessa. Tämän mahdollistaa GameWidget, joka koodimääränsä
	puolesta mielestämme voidaan laskea lisäominaisuudeksi. Samaan tyyliinhän olisi mahdollista toteuttaa mitä tahansa
	käyttöliittymiä. Käyttöliittymät muistavat oman tilansa, joten eri näkymän esittäminen ei aiheuta tietojen menetystä
	käyttöliittymässä.
	
	Vastaavan toiminnallisuuden toteuttaminen ilman GameWidgettiä, tai muuta vastaavaa luokkaa, vaatisi tolkuttoman määrän
	toisteista koodia MainWindow:ssa, koska kaikkien käyttöliittymien tilat pitäisi tallettaa erikseen MainWindowiin.
	Oikeanlaisilla tietorakenteella se olisi ehkä tehtävissä, mutta GameWidgetin tyyppisellä toteutuksella lopputulos on
	huomattavasti selkeämpi, helposti hallittavampi, ja sitä kautta laajennettavampi.

## Työnjako
Työnjako toimi mielestämme erittäin hyvin. Kommunikoimme harkkatyötä tehdessä paljon ja autoimmme toisiamme
ongelmanratkaisussa. Gitin committimäärät eivät täysin heijasta tätä, koska työn alkupuolella harrastimme jonkin verran
parikoodausta ja myös committien koko vaikuttaa niiden määrään.

Molemmilla meistä on melko paljon kursseija nyt syksyllä, minkä takia harkkatyö tehtiin vähän kiireellä. Pyrimme
kuitenkin kumpikin tekemään parhaamme, eikä kumpikaan joutunut tekemään ns. leijonanosaa työstä. Jatkossa varmaan
kannattaisi aloittaa harkkatöiden teko vähän aikaisemmin. Toisaalta, pyrimme alunperin aloittamaan miltei heti
tehtävänannon julkaisun jälkeen, mutta alkuperäisen koodipohjan ongelmat hidastivat alkuun pääsyä melko paljon.
