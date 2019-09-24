#include "xyskin.h"
#include <QFile>
#include <QApplication>
#include <QDomDocument>
#include <QTextStream>
#include <QDebug>

XYSkin *XYSkin::instance = Q_NULLPTR;
XYSkin::XYSkin(QObject *parent)
    : QObject(parent)
{
    initWithNoSkinFile();
}

void XYSkin::initWithNoSkinFile()
{
    QFont font;
    font.setPixelSize(18);
    defaultButtonStyle1 = new KeyButtonStyle;
    defaultButtonStyle1->textFont        = font;
    defaultButtonStyle1->backgroundColor = QColor("#FeFeFe");
    defaultButtonStyle1->textPen         = QPen(QColor("#101010"));
    defaultButtonStyle1->borderPen       = QPen(QColor("#BBBBBB"));
    defaultButtonStyle1->borderFont      = font;
    defaultButtonStyle1->backImage       = QPixmap();

    defaultButtonStyle1_pressed = new KeyButtonStyle(defaultButtonStyle1);
    defaultButtonStyle1_pressed->pressed = 1;
    defaultButtonStyle1_pressed->backgroundColor = QColor(0xa8, 0xa8, 0xa8, 0xff);

    defaultButtonStyle2 = new KeyButtonStyle;
    defaultButtonStyle2->textFont        = font;
    defaultButtonStyle2->backgroundColor = QColor("#DDDDDD");
    defaultButtonStyle2->textPen         = QPen(QColor("#101010"));
    defaultButtonStyle2->borderPen       = QPen(QColor("#BBBBBB"));
    defaultButtonStyle2->borderFont      = font;
    defaultButtonStyle2->backImage       = QPixmap();

    defaultButtonStyle2_pressed = new KeyButtonStyle(defaultButtonStyle2);
    defaultButtonStyle2_pressed->pressed = 1;
    defaultButtonStyle2_pressed->backgroundColor = QColor(0xa8, 0xa8, 0xa8, 0xff);

    font.setPixelSize(26);
    popupButtonStyle = new KeyButtonStyle;
    popupButtonStyle->textFont        = font;
    popupButtonStyle->backgroundColor = QColor("#FeFeFe");
    popupButtonStyle->textPen         = QPen(QColor("#101010"));
    popupButtonStyle->borderPen       = QPen(QColor("#BBBBBB"));
    popupButtonStyle->borderFont      = font;
    popupButtonStyle->backImage       = QPixmap();

    topColor = QColor("#fdfefe");
    font.setPixelSize(20);
    candidateTextFont = font;
    inputLettersFont  = font;
    candidateTextPen  = QPen(QColor("black"));
    candidateTextPressedBKColor = QColor(0xa8, 0xa8, 0xa8, 0xff);
    trianglePressedBKColor = QColor(0xa8, 0xa8, 0xa8, 0xff);
    bottomColor1 = QColor("#fdfefe");
    bottomColor2 = QColor("#EEEEEE");
    borderPen    = QPen(QColor(0x55, 0x55, 0x55, 0x50));
    bottomBKPixmap = QPixmap();

    QPen pen(QColor("black"));
    pen.setWidth(3);
    trianglePen = pen;
}

XYSkin *XYSkin::getInstance()
{
    if (instance == Q_NULLPTR)
    {
        instance = new XYSkin;
    }
    return instance;
}

XYSkin::~XYSkin()
{

}

KeyButtonStyle *XYSkin::getStyleByKey(int key, bool pressed)
{
    KeyButtonStyle *found = Q_NULLPTR;
    for (int i = 0; i < userDefButtonsStyle.size(); ++i)
    {
        if (key == userDefButtonsStyle.at(i)->key)
        {
            if ((userDefButtonsStyle.at(i)->pressed != 0) == pressed)
            {
                return userDefButtonsStyle.at(i);
            }
            else
            {
                found = userDefButtonsStyle.at(i);
            }
        }
    }
    if (found != Q_NULLPTR)
    {
        return found;
    }

    if ((key == Qt::Key_Shift
         || key == Qt::Key_Control
         || key == Qt::Key_Alt
         || key == Qt::Key_Meta
         || key == Qt::Key_Backspace
         || key == Qt::Key_Enter
         || key == Qt::Key_Return))
    {
        if (pressed)
        {
            return defaultButtonStyle2_pressed;
        }
        else
        {
            return defaultButtonStyle2;
        }
    }
    else
    {
        if (pressed)
        {
            return defaultButtonStyle1_pressed;
        }
        else
        {
            return defaultButtonStyle1;
        }
    }
}

bool XYSkin::loadSkinWithFile(const QString &filePath)
{
    QString path = filePath;
    QFile file(path);
    if(file.open(QFile::ReadOnly)) {
        QDomDocument document;
        QString strError;
        int errLin = 0, errCol = 0;
        if( !document.setContent(&file, false, &strError, &errLin, &errCol) ) {
            file.close();
            return false;
        } else if( document.isNull() ) {
            file.close();
            return false;
        } else {
            QDomElement root = document.documentElement();
            if (root.tagName() == "XYInputMethod") {
                qDeleteAll(userDefButtonsStyle);
                userDefButtonsStyle.clear();
                QDomElement element = root.firstChildElement();
                while(!element.isNull()) {
                    if (element.tagName() == "defaultButtonStyle1") {
                        loadButtonStyle(element, defaultButtonStyle1);
                    } else if (element.tagName() == "defaultButtonStyle1_pressed") {
                        loadButtonStyle(element, defaultButtonStyle1_pressed);
                    } else if (element.tagName() == "defaultButtonStyle2") {
                        loadButtonStyle(element, defaultButtonStyle2);
                    } else if (element.tagName() == "defaultButtonStyle2_pressed") {
                        loadButtonStyle(element, defaultButtonStyle2_pressed);
                    } else if (element.tagName() == "popupButtonStyle") {
                        loadButtonStyle(element, popupButtonStyle);
                    }  else if (element.tagName().startsWith("userDefButtonsStyle")) {
                        KeyButtonStyle *btn = new KeyButtonStyle;
                        userDefButtonsStyle.append(btn);
                        loadButtonStyle(element, btn);
                    } else if (element.tagName() == "topColor") {
                        loadColorStyle(element, topColor);
                    } else if (element.tagName() == "trianglePen") {
                        loadPenStyle(element, trianglePen);
                    } else if (element.tagName() == "trianglePressedBKColor") {
                        loadColorStyle(element, trianglePressedBKColor);
                    } else if (element.tagName() == "triangleBKPixmap") {
                        loadPixmapStyle(element, triangleBKPixmap);
                    } else if (element.tagName() == "triangleBKPressedPixmap") {
                        loadPixmapStyle(element, triangleBKPressedPixmap);
                    } else if (element.tagName() == "bottomColor1") {
                        loadColorStyle(element, bottomColor1);
                    } else if (element.tagName() == "bottomColor2") {
                        loadColorStyle(element, bottomColor2);
                    } else if (element.tagName() == "borderPen") {
                        loadPenStyle(element, borderPen);
                    } else if (element.tagName() == "inputLettersColor") {
                        loadColorStyle(element, inputLettersColor);
                    } else if (element.tagName() == "inputLettersFont") {
                        loadFontStyle(element, inputLettersFont);
                    } else if (element.tagName() == "candidateTextPen") {
                        loadPenStyle(element, candidateTextPen);
                    } else if (element.tagName() == "candidateTextPressedBKColor") {
                        loadColorStyle(element, candidateTextPressedBKColor);
                    } else if (element.tagName() == "candidateTextFont") {
                        loadFontStyle(element, candidateTextFont);
                    } else if (element.tagName() == "topBKPixmap") {
                        loadPixmapStyle(element, topBKPixmap);
                    } else if (element.tagName() == "bottomBKPixmap") {
                        loadPixmapStyle(element, bottomBKPixmap);
                    }
                    element = element.nextSiblingElement();
                }
            }
            file.close();
        }
    } else {
        return false;
    }
    emit skinChanged();
    return true;
}

bool XYSkin::writeSkinToFile(const QString &filePath)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("XYInputMethod");
    doc.appendChild(root);

    // 写入按钮风格
    writeButtonStyle(doc, root, defaultButtonStyle1, "defaultButtonStyle1");
    writeButtonStyle(doc, root, defaultButtonStyle1_pressed, "defaultButtonStyle1_pressed");
    writeButtonStyle(doc, root, defaultButtonStyle2, "defaultButtonStyle2");
    writeButtonStyle(doc, root, defaultButtonStyle2_pressed, "defaultButtonStyle2_pressed");
    writeButtonStyle(doc, root, popupButtonStyle, "popupButtonStyle");

    for (int i = 0; i < userDefButtonsStyle.size(); ++i)
    {
        writeButtonStyle(doc, root, userDefButtonsStyle.at(i), QString("userDefButtonsStyle%1").arg(i));
    }

    writeColorStyle(doc, root, topColor, "topColor");
    writePenStyle(doc, root, trianglePen, "trianglePen");
    writeColorStyle(doc, root, trianglePressedBKColor, "trianglePressedBKColor");
    writePixmapStyle(doc, root, triangleBKPixmap, "triangleBKPixmap");
    writePixmapStyle(doc, root, triangleBKPressedPixmap, "triangleBKPressedPixmap");
    writeColorStyle(doc, root, bottomColor1, "bottomColor1");
    writeColorStyle(doc, root, bottomColor2, "bottomColor2");
    writePenStyle(doc, root, borderPen, "borderPen");
    writeColorStyle(doc, root, inputLettersColor, "inputLettersColor");
    writeFontStyle(doc, root, inputLettersFont, "inputLettersFont");
    writePenStyle(doc, root, candidateTextPen, "candidateTextPen");
    writeColorStyle(doc, root, candidateTextPressedBKColor, "candidateTextPressedBKColor");

    // topImage
    writePixmapStyle(doc, root, topBKPixmap, "topBKPixmap");

    // backImage
    writePixmapStyle(doc, root, bottomBKPixmap, "bottomBKPixmap");

    // candidateTextFont
    writeFontStyle(doc, root, candidateTextFont, "candidateTextFont");

    // 写入文件
    QString path = filePath;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text)) {
        qDebug() << "Open setting file failed! so can't save setting!";
        return false;
    }
    QTextStream out(&file);
    doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();
    return true;
}

void XYSkin::loadIntStyle(const QDomElement &parent, int &value)
{
    value = parent.attribute("value").toInt(Q_NULLPTR, 16);
}

void XYSkin::loadPixmapStyle(const QDomElement &parent, QPixmap &pixmap)
{
    pixmap.load(parent.attribute("filepath"));
}

void XYSkin::loadColorStyle(const QDomElement &parent, QColor &color)
{
    QString colorname = parent.attribute("color");
    QString alphaString;
    if (colorname.size() < 7)
    {
        if (!colorname.isEmpty())
        {
            color = QColor(colorname);
        }
        return;
    }
    color = QColor(colorname.mid(0, 7));
    if (colorname.size() > 7)
    {
        alphaString = colorname.mid(7);
    }
    if (alphaString.size() == 2)
    {
        color.setAlpha(alphaString.toInt(Q_NULLPTR, 16));
    }
}

void XYSkin::loadFontStyle(const QDomElement &parent, QFont &font)
{
    if (!parent.attribute("family").isEmpty())
    {
        font.setFamily(parent.attribute("family"));
    }
    if (!parent.attribute("pixelSize").isEmpty())
    {
        font.setPixelSize(parent.attribute("pixelSize").toInt());
    }
    if (!parent.attribute("weight").isEmpty())
    {
        font.setWeight(parent.attribute("weight").toInt());
    }
}

void XYSkin::loadPenStyle(const QDomElement &parent, QPen &pen)
{
    if (!parent.attribute("width").isEmpty())
    {
        pen.setWidth(parent.attribute("width").toInt());
    }
    if (!parent.attribute("color").isEmpty())
    {
        QColor color;
        loadColorStyle(parent, color);
        pen.setColor(color);
    }
}

void XYSkin::loadButtonStyle(const QDomElement &parent, KeyButtonStyle *btn)
{
    QDomNodeList it = parent.childNodes();
    for (int i = 0; i < it.size(); ++i)
    {
        if (it.at(i).toElement().tagName() == "key") {
            loadIntStyle(it.at(i).toElement(), btn->key);
        } else if (it.at(i).toElement().tagName() == "pressed") {
            loadIntStyle(it.at(i).toElement(), btn->pressed);
        } else if (it.at(i).toElement().tagName() == "textFont") {
            loadFontStyle(it.at(i).toElement(), btn->textFont);
        } else if (it.at(i).toElement().tagName() == "backgroundColor") {
            loadColorStyle(it.at(i).toElement(), btn->backgroundColor);
        } else if (it.at(i).toElement().tagName() == "textPen") {
            loadPenStyle(it.at(i).toElement(), btn->textPen);
        } else if (it.at(i).toElement().tagName() == "borderPen") {
            loadPenStyle(it.at(i).toElement(), btn->borderPen);
        } else if (it.at(i).toElement().tagName() == "borderFont") {
            loadFontStyle(it.at(i).toElement(), btn->borderFont);
        } else if (it.at(i).toElement().tagName() == "backImage") {
            loadPixmapStyle(it.at(i).toElement(), btn->backImage);
        }
    }
}

void XYSkin::writeIntStyle(QDomDocument &doc, QDomElement &parent, int key, const QString &title)
{
    QDomElement keyElement = doc.createElement(title);
    parent.appendChild(keyElement);
    QDomAttr keyAttr = doc.createAttribute("value");
    keyAttr.setValue(QString::number(key, 16));
    keyElement.setAttributeNode(keyAttr);
}

void XYSkin::writePixmapStyle(QDomDocument &doc, QDomElement &parent, const QPixmap &pixmap, const QString &title)
{
    QDomElement pixmapElement = doc.createElement(title);
    parent.appendChild(pixmapElement);
    QDomAttr filepath = doc.createAttribute("filepath");
    filepath.setValue("filepath");
    pixmapElement.setAttributeNode(filepath);
}

void XYSkin::writeColorStyle(QDomDocument &doc, QDomElement &parent, const QColor &color, const QString &title, bool isAttribute)
{
    if (isAttribute)
    {
        QDomAttr name = doc.createAttribute(title);
        name.setValue(color.name() + QString::number(color.alpha(), 16));
        parent.setAttributeNode(name);
    }
    else
    {
        QDomElement colorElement = doc.createElement(title);
        parent.appendChild(colorElement);
        QDomAttr name = doc.createAttribute("color");
        name.setValue(color.name() + QString::number(color.alpha(), 16));
        colorElement.setAttributeNode(name);
    }
}

void XYSkin::writeFontStyle(QDomDocument &doc,QDomElement &parent, const QFont &font, const QString &title)
{
    QDomElement fontElement = doc.createElement(title);
    parent.appendChild(fontElement);
    QDomAttr name = doc.createAttribute("family");
    name.setValue(font.family());
    fontElement.setAttributeNode(name);
    QDomAttr pixelSize = doc.createAttribute("pixelSize");
    pixelSize.setValue(QString::number(font.pixelSize()));
    fontElement.setAttributeNode(pixelSize);
    QDomAttr weight = doc.createAttribute("weight");
    weight.setValue(QString::number(font.weight()));
    fontElement.setAttributeNode(weight);
}

void XYSkin::writePenStyle(QDomDocument &doc, QDomElement &parent, const QPen &pen, const QString &title)
{
    QDomElement penElement = doc.createElement(title);
    parent.appendChild(penElement);
    writeColorStyle(doc, penElement, pen.color(), "color", true);
    QDomAttr width = doc.createAttribute("width");
    width.setValue(QString::number(pen.width()));
    penElement.setAttributeNode(width);
}

void XYSkin::writeButtonStyle(QDomDocument &doc, QDomElement &parent, KeyButtonStyle *btn, const QString &title)
{
    QDomElement btnstyle = doc.createElement(title);
    parent.appendChild(btnstyle);
    // key
    writeIntStyle(doc, btnstyle, btn->key, "key");

    // pressed
    writeIntStyle(doc, btnstyle, btn->pressed, "pressed");

    // font
    writeFontStyle(doc, btnstyle, btn->textFont, "textFont");

    // backgroundcolor
    writeColorStyle(doc, btnstyle, btn->backgroundColor, "backgroundColor");

    // textPen
    writePenStyle(doc, btnstyle, btn->textPen, "textPen");

    // borderPen
    writePenStyle(doc, btnstyle, btn->borderPen, "borderPen");

    // borderFont
    writeFontStyle(doc, btnstyle, btn->borderFont, "borderFont");

    // backImage
    writePixmapStyle(doc, btnstyle, btn->backImage, "backImage");
}
