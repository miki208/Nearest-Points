///
/// Svaki algoritam koji cemo implementirati treba da:
/// 1. Nasledi AlgorithmBase (koji predstavlja apstrakciju koja vodi racuna o animaciji i iscrtavanju)
/// 2. Da implementira metod "runAlgorithm" u kom ce biti implementacija algoritma.
/// 3. Da implementira metod "drawAlgorithm" u kom ce biti implementirano iscrtavanje
/// 4. Da implementira metod "runNaiveAlgorithm" u kom ce biti implementacija algoritma grube sile
///
/// Svaki put kada se promeni stanje algoritma (kada je potrebno promeniti crtez),
/// potrebno je pozvati metod AlgorithmBase_updateCanvasAndBlock();
/// Ovo je zapravo makro, koji pozove metod updateCanvasAndBlock() i u slucaju da
/// detektuje da algoritam treba da se zaustavi (jedna bool promenljiva koja se
/// postavlja na true kada kosirnik klikne "Stop"), samo se zamenjuje komandom
/// return;
///
/// Razlog za ovaj mehanizam je to sto se algoritam izvrsava u okviru svoje niti i nije bezbedno
/// (sa aspekta resursa) ubiti tu nit dok se ne zavrsi. Na ovaj nacin, svaki put
/// kada se vrsi azuriranje crteza, proverava se i stanje pomenute logicke promenljive
/// i ukoliko ona signalizira da treba da se zaustavimo, na mestu azuriranja crteza ce
/// se naci komanda return; i na taj nacin smo postigli efekat koji smo zeleli
/// (izaslo se iz funkcije koja se izvrsava u okviru niti za animaciju)

#ifndef ALGORITHMBASE_H
#define ALGORITHMBASE_H

#include <QSemaphore>
#include <QWidget>

#include <QPen>
#include <QPainter>

#include <vector>

#define AlgorithmBase_updateCanvasAndBlock() \
    if(!_pThread) \
    { \
       ; \
    } \
    else if (updateCanvasAndBlock()) \
    { \
        return; \
    }

class AnimationThread;

class AlgorithmBase: public QObject
{
    Q_OBJECT

private:
    static int constexpr INVALID_TIMER_ID = -1;
    static int constexpr DRAWING_BORDER = 10;
public:
    AlgorithmBase(QWidget *pRender, int delayMs);

    ///
    /// \brief runAlgorithm - funkcija koju ce svaka podklasa implementirati,
    ///         a koja predstavlja izvrsavanje konkretnog algoritma
    ///
    virtual void runAlgorithm() = 0;
    virtual void drawAlgorithm(QPainter &painter) const = 0;
    virtual void runNaiveAlgorithm() = 0;

    ///
    /// \brief startAnimation - funkcija za pokretanje animacije
    ///     1. Pravi i pokrece nit za konkretan algoritam
    ///     2. Pokrece tajmer
    ///
    void startAnimation();

    ///
    /// \brief pauseOrContinueAnimation - pauziranja, odnosno ponovno pokretanje animacije
    ///         Ako je tajmer pokrenut, zaustavlja ga
    ///         Ako tajmer nije pokrenut, pokrece ga
    ///
    void pauseOrContinueAnimation();

    ///
    /// \brief resetAnimation - zaustavljanje animacije
    ///     1. Postavljanje logicke promenljive na true
    ///     2. Pustanje semafora (da bi se animacija "nastavila", tj. da bi se doslo do updateCanvasAndBlock())
    ///         2.1. U updateCanvasAndBlock ce se detektovati da je logicka promenljiva true i algoritam ce se zaustaviti
    ///     3. Oslobadjanje odgovarajucih resursa
    ///
    void resetAnimation();

    ///
    /// \brief nextStep - izvrsavanje algoritma korak po korak
    ///     1. Zaustavljanje tajmera (da bi se zaustavilo kontinuirano izvrsavanje algoritma)
    ///     2. Oslobadjanje semafora (da bi se izvrsio jedan korak algoritma)
    ///
    void nextStep();

    ///
    /// \brief changeDelay - funkcija za promenu brzine animacije
    /// \param delayMs - pauza izmedju dva koraka, izrazena u ms
    ///
    void changeDelay(int delayMs);

signals:
    void animationFinished();

protected:
    static int constexpr DEFAULT_POINTS_NUM = 20;

    ///
    /// \brief updateCanvasAndBlock - azuriranje crteza i blokiranje dok se semafor ne oslobodi
    ///     1. Azuriranje crteza iz renderarea klase
    ///     2. Blokiranje semafora (semafor se oslobadja u tajmeru, tako da se na taj nacin
    ///         simulira animacija)
    /// \return true ako animacija treba da se zaustavi, false u suprotnom
    ///
    bool updateCanvasAndBlock();

    std::vector<QPoint> generateRandomPoints(int pointsNum = DEFAULT_POINTS_NUM);
    std::vector<QPoint> readPointsFromFile(std::string fileName);
    std::vector<QLineF> generateRandomLines(int linesNum = DEFAULT_POINTS_NUM/2);

    /* Paarametri za iscrtavanje*/
    QWidget *_pRenderer;
    bool _destroyAnimation;

    /* Nit koja izvrsava algoritam */
    AnimationThread *_pThread;
private:
    ///
    /// \brief timerEvent - funkcija koja se poziva na svakih _delayMs ms.
    ///     U njoj samo oslobadjamo semafor i na taj nacin omogucavamo da se predje na sledeci
    ///     korak algoritma
    /// \param event
    ///
    void timerEvent(QTimerEvent *event);

    /* Parametri za animaciju */
    int _delayMs;
    int _timerId;
    QSemaphore _semaphore;
};

#endif // ALGORITHMBASE_H
