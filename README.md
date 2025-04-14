# poker

Celem projektu było zbudowanie sieciowej gry poker. Serwer odgrywa role krupiera. Po rozpoczęciu gry tasuje karty, a następnie rozdaje je każdemu z
graczy. Po rozdaniu kart graczom, wystawia karty na stół, a na koniec sprawdza, który z graczy wygrał. Serwer
bazuje na protokole TCP oraz zdecydowano, że będzie opierał się na adresach z rodziny IPv6. Do zwielokrotnienia
wejścia/wyjścia wykorzystano funkcję poll().

Klient uruchamia program z adresem serwera jako parametr. Po połączeniu z serwerem odpowiedniej ilości graczy,
każdy z nich jest proszony o podanie nickname’u z którym będzie kojarzony podczas rozgrywki. Po tym jak wszyscy
gracze wpiszą swoją nazwe, gra się rozpoczyna. Zdecydowano się na napisanie pokera „all-in”, to znaczy, że po
rozdaniu kart wszycy gracze wystawiają je na stół, i liczą, że po wystawieniu na stół pozostałych kart przez krupiera,
wygrana będzię należała właśnie do niego.
