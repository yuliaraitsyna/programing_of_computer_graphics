# Color Picker
Приложение разработано с целью выполения лабороторной работы №1.

Был использован язык программирования С++ и среда программирования Desktop Qt 5.15 MinGW 32-bit".

Основные объекты приложения:
- Кнопка выбора цвета
- Одноцветный виджет
- Три слайлера
- Названия цветовых систем: CMYK, RGB, HSL
- Три (четыре поля ввода)

Реализован функционал:
- При нажатии кнопки, расположенной выше цветового виджета, появляется диалоговое окно для самостоятельного выбора цвета.
- Цветовой виджет отображает текущий цвет.
- Под цветовым виджетом расподожены три слайдера для выбора цвета в цветовой модели RGB.
- При изменении значений слайдеров меняется текущий цвет и высчитываются значения в остальных цветовых моделях.
- При изменении значений в ComboBox после нажатия клавиши ввод (Enter) меняется текущий цвет и значения в других цветовых моделях.\

Библиотеки:
- QWidget
 - QLabel
 - QColor
 - QColorDialog
 - QPushButton
 - QComboBox
 - QSlider
 
Файлы:
- mainwindow.h
- mainwindow.cpp
- main.cpp
- остальные файлы являются вспомогательными
