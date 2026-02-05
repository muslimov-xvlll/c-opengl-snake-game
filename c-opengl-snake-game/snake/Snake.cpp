#include <GL/glut.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

int choose_level;
int window;

// Определение структуры Point для хранения координат x и y
typedef struct {
    int x;
    int y;
} Point;

Point snake[100]; // Массив точек, представляющих змейку
int snake_length = 1;

Point fruit; // Точка, представляющая фрукт
Point goldenFruit; // Точка, представляющая золотое яблоко
Point pinkyFruit; // Точка, представляющая серое яблоко
Point blueFruit; // Точка, представляющая синее яблоко

bool blueFruitExists = false; // Переменная для отслеживания наличия синего яблока
int blueFruitFrameCount = 0; // Счетчик кадров для синего яблока
bool shieldActive = false; // Переменная для отслеживания активности щита
int shieldFrameCount = 0; // Счетчик кадров для щита

bool goldenFruitExists = false; // Переменная для отслеживания наличия золотого яблока
bool pinkyFruitExists = false; // Переменная для отслеживания наличия серого яблока
int goldenFruitFrameCount = 0; // Счетчик кадров для золотого яблока
int pinkyFruitFrameCount = 0; // Счетчик кадров для серого яблока

//переменные для отслеживания текущего альфа-значения яблок
float goldenFruitAlpha = 1.0;
float pinkyFruitAlpha = 1.0;
float blueFruitAlpha = 1.0;

int direction = 0; // Переменная для хранения текущего направления движения
int score = 0; // количество яблок, которые съели
bool gameOver = false; // Переменная для отслеживания состояния игры

// Функция для сравнения двух точек
bool comparePoints(Point a, Point b) {
    return a.x == b.x && a.y == b.y;
}

//Процедура для отрисовки текста на экране
void drawText(const char* text, float x, float y, void* font) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

// Функция для генерации новой позиции яблока
void generateFruit() {
    srand(time(0));
    while (true) {
        fruit.x = rand() % 20;
        fruit.y = rand() % 20;
        // Проверяем, что яблоко не появляется в теле змейки или в центральном квадрате
        bool inSnake = false;
        for (int i = 0; i < snake_length; i++) {
            if (comparePoints(snake[i], fruit)) {
                inSnake = true;
                break;
            }
        }
        if (inSnake || (choose_level == 3 && fruit.x >= 8 && fruit.x <= 12 && fruit.y >= 8 && fruit.y <= 12))
            continue;
        break;
    }
    // Генерируем золотое яблоко
    if (rand() % 10 == 0 && !goldenFruitExists && !pinkyFruitExists && !blueFruitExists) { // 10% шанс появления золотого яблока
        while (true) {
            goldenFruit.x = rand() % 20;
            goldenFruit.y = rand() % 20;
            // Проверяем, что золотое яблоко не появляется в теле змейки, в центральном квадрате или на месте других яблок
            bool inSnake = false;
            for (int i = 0; i < snake_length; i++) {
                if (comparePoints(snake[i], goldenFruit)) {
                    inSnake = true;
                    break;
                }
            }
            if (inSnake || (choose_level == 3 && goldenFruit.x >= 8 && goldenFruit.x <= 12 && goldenFruit.y >= 8 && goldenFruit.y <= 12) || comparePoints(goldenFruit, fruit))
                continue;
            break;
        }
        goldenFruitExists = true;
        goldenFruitFrameCount = 0; // Устанавливаем счетчик кадров в 0
    }
    // Генерируем розовое яблоко
    else if (rand() % 10 == 0 && !goldenFruitExists && !pinkyFruitExists && !blueFruitExists) { // 10% шанс появления серого яблока
        while (true) {
            pinkyFruit.x = rand() % 20;
            pinkyFruit.y = rand() % 20;
            // Проверяем, что серое яблоко не появляется в теле змейки, в центральном квадрате или на месте других яблок
            bool inSnake = false;
            for (int i = 0; i < snake_length; i++) {
                if (comparePoints(snake[i], pinkyFruit)) {
                    inSnake = true;
                    break;
                }
            }
            if (inSnake || (choose_level == 3 && pinkyFruit.x >= 8 && pinkyFruit.x <= 12 && pinkyFruit.y >= 8 && pinkyFruit.y <= 12) || comparePoints(pinkyFruit, fruit))
                continue;
            break;
        }
        pinkyFruitExists = true;
        pinkyFruitFrameCount = 0; // Устанавливаем счетчик кадров в 0
    }
    else if (rand() % 10 == 0 && !goldenFruitExists && !pinkyFruitExists && !blueFruitExists) { // 10% шанс появления синего яблока
        while (true) {
            blueFruit.x = rand() % 20;
            blueFruit.y = rand() % 20;
            // Проверяем, что синее яблоко не появляется в теле змейки, в центральном квадрате или на месте других яблок
            bool inSnake = false;
            for (int i = 0; i < snake_length; i++) {
                if (comparePoints(snake[i], blueFruit)) {
                    inSnake = true;
                    break;
                }
            }
            if (inSnake || (choose_level == 3 && blueFruit.x >= 8 && blueFruit.x <= 12 && blueFruit.y >= 8 && blueFruit.y <= 12) || comparePoints(blueFruit, fruit))
                continue;
            break;
        }
        blueFruitExists = true;
        blueFruitFrameCount = 0; // Устанавливаем счетчик кадров в 0
    }
}


// Процедура для рисования квадрата в заданной точке
void drawSquare(Point point) {
    glBegin(GL_QUADS);
    glVertex2i(point.x, point.y);
    glVertex2i(point.x + 1, point.y);
    glVertex2i(point.x + 1, point.y + 1);
    glVertex2i(point.x, point.y + 1);
    glEnd();
}

void drawBordersAndCenterSquare() {
    glClearColor(0.79, 0.68, 0.13, 1.0);
    glColor3f(1.0, 1.0, 1.0); // Устанавливаем белый цвет
    glLineWidth(20);
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 0);
    glVertex2i(0, 20);
    glVertex2i(20, 20);
    glVertex2i(20, 0);
    glEnd();
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2i(8, 8);
    glVertex2i(8, 12);
    glVertex2i(12, 12);
    glVertex2i(12, 8);
    glEnd();
}

void drawBorders() {
    glClearColor(0.1, 0.59, 0.65, 1.0);
    glColor3f(0.65, 0.1, 0.1); // Устанавливаем белый цвет
    glLineWidth(20);
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 0);
    glVertex2i(0, 20);
    glVertex2i(20, 20);
    glVertex2i(20, 0);
    glEnd();
}

// Функция для рисования всей сцены
void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver) {
        // Координаты и размеры прямоугольника
        float rectX = 5.0;
        float rectY = 7.5;
        float rectWidth = 10.0;
        float rectHeight = 5.0;

        // Рисуем белый прямоугольник
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
        glVertex2f(rectX, rectY);
        glVertex2f(rectX + rectWidth, rectY);
        glVertex2f(rectX + rectWidth, rectY + rectHeight);
        glVertex2f(rectX, rectY + rectHeight);
        glEnd();

        // Рисуем сообщение о завершении игры
        glColor3f(0.0, 0.0, 0.0); // Черный цвет для текста
        char gameOverText[100];
        sprintf_s(gameOverText, "GAME OVER! SCORE: %d PRESS R FOR RESTART, ESC FOR EXIT", score);
        drawText(gameOverText, 6, 10, GLUT_BITMAP_HELVETICA_18);
    }
    else {
        glColor3f(0.0, 1.0, 0.0); // Зеленый цвет для змейки

        for (int i = 0; i < snake_length; i++) {
            if (i == 0 && shieldActive) {
                glColor3f(0.0, 0.0, 1.0); // Синий цвет для головы змейки, когда щит активен
            }
            drawSquare(snake[i]);
            if (i == 0 && shieldActive) {
                glColor3f(0.0, 1.0, 0.0); // Возвращаем зеленый цвет для остальной части змейки
            }
        }
        glColor3f(1.0, 0.0, 0.0); // Красный цвет для яблока
        drawSquare(fruit);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (goldenFruitExists) {
            glColor4f(1.0, 1.0, 0.0, goldenFruitAlpha); // Используем альфа-значение для золотого яблока
            drawSquare(goldenFruit);
        }
        if (pinkyFruitExists) {
            glColor4f(0.97, 0.04, 0.94, pinkyFruitAlpha); // Используем альфа-значение для серого яблока
            drawSquare(pinkyFruit);
        }
        if (blueFruitExists) {
            glColor4f(0.0, 0.0, 1.0, blueFruitAlpha); // Синий цвет для синего яблока
            drawSquare(blueFruit);
        }
        glDisable(GL_BLEND);
        switch (choose_level) {
        case 1: glClearColor(0.22, 0.59, 0.28, 1.0); break;
        case 2: drawBorders(); break;
        case 3: drawBordersAndCenterSquare(); break;
        default: break;
        }

        // Отображаем текущий счет
        char scoreText[20];
        sprintf_s(scoreText, "SCORE: %d", score);
        float x = 9;
        float y = 1;

        // Рисуем обводку (черный цвет)
        glColor3f(0.0, 0.0, 0.0);
        for (float dx = -0.02; dx <= 0.02; dx += 0.01) {
            for (float dy = -0.02; dy <= 0.02; dy += 0.01) {
                drawText(scoreText, x + dx, y + dy, GLUT_BITMAP_TIMES_ROMAN_24);
            }
        }


        // Рисуем основной текст
        glColor3f(1.0, 1.0, 1.0); // Белый цвет для текста
        drawText(scoreText, 9, 1, GLUT_BITMAP_TIMES_ROMAN_24);

       
    }

    glutSwapBuffers();
}

/// Функция таймера для обновления сцены
void timer(int value) {
    if (gameOver) {
        return; // Если игра окончена, не обновляем сцену
    }

    Point next = snake[0];
    switch (direction) {
    case 0: next.y += 1; break;
    case 1: next.x += 1; break;
    case 2: next.y -= 1; break;
    case 3: next.x -= 1; break;
    }

    if (choose_level == 1) {
        if (next.x < 0) next.x = 19;
        if (next.x > 19) next.x = 0;
        if (next.y < 0) next.y = 19;
        if (next.y > 19) next.y = 0;
    }

    // Проверка на столкновение с границами и самим собой
    bool inSnake = false;
    for (int i = 0; i < snake_length; i++) {
        if (comparePoints(snake[i], next)) {
            inSnake = true;
            break;
        }
    }

    if (next.x < 0 || next.x > 19 || next.y < 0 || next.y > 19 || (!shieldActive && inSnake) || (choose_level == 3 && next.x >= 8 && next.x < 12 && next.y >= 8 && next.y < 12)) {
        gameOver = true; // Устанавливаем флаг gameOver
        shieldActive = false;
    }

    if (gameOver) {
        glutPostRedisplay();
        return; // Если игра окончена, не обновляем сцену
    }

    if (comparePoints(next, fruit)) {
        score++;
        generateFruit(); // Генерируем новую позицию для яблока
        snake_length++; // Увеличиваем длину змейки
    }
    else if (goldenFruitExists && comparePoints(next, goldenFruit)) {
        score += 5;
        goldenFruitExists = false; // Удаляем золотое яблоко
        snake_length++; // Увеличиваем длину змейки
    }
    else if (pinkyFruitExists && comparePoints(next, pinkyFruit)) {
        snake_length = 1; // Уменьшаем змейку до 1 блока
        pinkyFruitExists = false; // Удаляем серое яблоко
    }
    else {
        for (int i = snake_length - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
    }

    if (goldenFruitExists) {
        goldenFruitFrameCount++;
        goldenFruitAlpha = 0.5 + 0.5 * sin(glutGet(GLUT_ELAPSED_TIME) / 500.0); // Изменяем альфа-значение золотого яблока
        if (goldenFruitFrameCount > 50) {
            goldenFruitExists = false; // Удаляем золотое яблоко, если оно не было съедено в течение 50 кадров
        }
    }
    if (pinkyFruitExists) {
        pinkyFruitFrameCount++;
        pinkyFruitAlpha = 0.5 + 0.5 * sin(glutGet(GLUT_ELAPSED_TIME) / 500.0); // Изменяем альфа-значение серого яблока
        if (pinkyFruitFrameCount > 50) {
            pinkyFruitExists = false; // Удаляем серое яблоко, если оно не было съедено в течение 50 кадров
        }
    }
    if (blueFruitExists) {
        blueFruitFrameCount++;
        blueFruitAlpha = 0.5 + 0.5 * sin(glutGet(GLUT_ELAPSED_TIME) / 500.0); // Изменяем альфа-значение синего яблока
        if (blueFruitFrameCount > 50) {
            blueFruitExists = false; // Удаляем синее яблоко, если оно не было съедено в течение 50 кадров
        }
    }
    if (blueFruitExists && comparePoints(next, blueFruit)) {
        shieldActive = true; // Активируем щит
        shieldFrameCount = 0; // Устанавливаем счетчик кадров щита в 0
        blueFruitExists = false; // Удаляем синее яблоко
    }
    if (shieldActive) {
        shieldFrameCount++;
        if (shieldFrameCount > 50) {
            shieldActive = false; // Деактивируем щит, если он был активен в течение 50 кадров
        }
    }

    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = next;
    glutTimerFunc(100, timer, 0);
    glutPostRedisplay();
}



// Функция для обработки нажатий клавиш
void keyboard(int key, int x, int y) {
    if (gameOver) {
        switch (key) {
        case GLUT_KEY_UP: break;
        case GLUT_KEY_RIGHT: break;
        case GLUT_KEY_DOWN: break;
        case GLUT_KEY_LEFT: break;
        }
    }
    if (snake_length == 1) {
        switch (key) { // Изменяем направление в зависимости от нажатой клавиши (если змейка в 1 блок)
        case GLUT_KEY_UP:  direction = 0; break;
        case GLUT_KEY_RIGHT:  direction = 1; break;
        case GLUT_KEY_DOWN:  direction = 2; break;
        case GLUT_KEY_LEFT:  direction = 3; break;
        }
    }
    else {
        switch (key) { // Изменяем направление в зависимости от нажатой клавиши (если змейка больше одного блока)
        case GLUT_KEY_UP: if (direction != 2) direction = 0; break;
        case GLUT_KEY_RIGHT: if (direction != 3) direction = 1; break;
        case GLUT_KEY_DOWN: if (direction != 0) direction = 2; break;
        case GLUT_KEY_LEFT: if (direction != 1) direction = 3; break;
        }
    }
}

void showMenu() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("- - - - - Добро пожаловать в Змейку! - - - - -\n");
    printf("Для движения используйте стрелки. Для выбора уровня введите номер уровня\n");
    printf("Выберите уровень: \n");
    printf("1 - без полей\n");
    printf("2 - поля по бокам\n");
    printf("3 - поля по бокам и пристройка в центре\n");
    scanf_s("%d", &choose_level);
}

void handleKeypress(unsigned char key, int x, int y) {
    if (gameOver) {
        switch (key) {
        case 27: // ESC key
            exit(0);
            break;
        case 'r':
        case 'R':
            gameOver = false;
            direction = 0;
            score = 0;
            snake_length = 1;
            snake[0].x = 5;
            snake[0].y = 5;
            generateFruit();
            glutTimerFunc(100, timer, 0);
            break;
        }
    }
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    showMenu(); // Показываем меню перед началом игры
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    window = glutCreateWindow("Snake Game");
    glutFullScreen(); // Делаем окно полноэкранным
    glOrtho(0, 20, 0, 20, -1, 1);
    glutDisplayFunc(draw);
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(100, timer, 0);
    snake[0].x = 5;
    snake[0].y = 5;
    generateFruit(); // Генерируем начальную позицию для яблока
    glutMainLoop();
    return 0;
}