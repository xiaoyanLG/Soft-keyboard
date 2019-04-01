/**
 * @file
 * Qt/E 4 input method and input panel
 *
 * Verification using sylixos(tm) real-time operating system
 */

/*
 * Copyright (c) 2006-2013 SylixOS Group.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Han.hui <sylixos@gmail.com>
 *
 */

#include "xygooglepinyin.h"
#include <QApplication>
#include <pinyinime.h>
#include <string.h>

using namespace ime_pinyin;

#define DICT_PATH           "dict_pinyin.dat"
#define DICT_USER_PATH      "dict_pinyin_user.dat"

// fix cand_len
static size_t fix_cand_len (size_t cand, int output_len)
{
#define MAX_SINGLE 40   // single hz MAX is 40
    size_t i;
    size_t decode_len;
    size_t single = 0;
    size_t multi = 0;
    char16 *cand_buf = new char16[output_len];

    im_get_sps_str(&decode_len);
    if (decode_len == 1) { // only one spell char
        if (cand > 10) {
            cand = 10; // max is 10.
        }
        delete cand_buf;
        return  cand;
    }

    for (i = 0; i < cand; i++) {
        im_get_candidate(i, cand_buf, output_len);
        if (strlen((char *)cand_buf) > 2) {
            multi++;
        } else {
            single++;
            if (single > MAX_SINGLE) {
                break;
            }
        }
    }

    cand = multi + single;
    delete cand_buf;
    return  cand;
}

// constructor
pinyin_im::pinyin_im(QObject *parent) :
    QObject(parent), enable(false)
{
}

// input panel destructor
pinyin_im::~pinyin_im()
{
}

// init dict database
bool pinyin_im::init(const QString &dir, int max_spell_len, int max_output_len)
{
    bool  	ret;
    QString app_dir = dir;
    if (app_dir.isEmpty()) {
        app_dir = qApp->applicationDirPath() + "/dict";
    }
    ret = im_open_decoder(QString("%1/" DICT_PATH).arg(app_dir).toLocal8Bit().data(),
                          QString("%1/" DICT_USER_PATH).arg(app_dir).toLocal8Bit().data());
    enable = ret;
    if (ret == false) {
        return ret;
    }

    im_set_max_lens(max_spell_len /* input pinyin max len */,
                    max_output_len /* max output Chinese character string len */);

    reset_search();

    spell_len = max_spell_len;
    output_len = max_output_len;

    return  ret;
}

// deinit dict database
void pinyin_im::deinit()
{
    im_close_decoder();
}

// flush dict cache
void pinyin_im::flush_cache()
{
    im_flush_cache();
}

// add a char to search
unsigned pinyin_im::search(const QString &spell)
{
    if (!enable)
    {
        return 0;
    }
    QByteArray bytearray;
    char    *pinyin;

    bytearray = spell.toUtf8();
    pinyin = bytearray.data();
    bytearray.size();

    size_t cand = im_search(pinyin, bytearray.size());

//    if (im_get_fixed_len()) {  // if fixed at least 1, candidate 0(whole line) is not display.
//        cand--;
//    }

    cand = fix_cand_len(cand, output_len);

    return  (unsigned)cand;
}

// delete a char to search
unsigned pinyin_im::del_search(unsigned pos)
{
    if (!enable)
    {
        return 0;
    }
    size_t cand = im_delsearch(pos, false, false);

//    if (im_get_fixed_len()) { // if fixed at least 1, candidate 0(whole line) is not display.
//        cand--;
//    }

    cand = fix_cand_len(cand, output_len);

    return  (unsigned)cand;
}

// reset search string
void pinyin_im::reset_search()
{
    if (!enable)
    {
        return;
    }
    im_reset_search();
}

// get current search position
unsigned pinyin_im::cur_search_pos()
{
    const uint16 *start_pos;
    size_t  pos_len;
    size_t  fixed_len = im_get_fixed_len();

    pos_len = im_get_spl_start_pos(start_pos);
    if (fixed_len <= pos_len) {
        return  (start_pos[fixed_len]);
    }

    return  0;
}

// get a candidate
QString pinyin_im::get_candidate(unsigned index)
{
    char16 *cand_buf = new char16[output_len];
    char16 *cand;
    QString cand_str;

//    if (im_get_fixed_len()) { // if fixed at least 1, candidate 0(whole line) is not display.
//        index++;
//    }

    cand = im_get_candidate(index, cand_buf, output_len);
    if (cand) {
        cand_str = QString::fromUtf16(cand); // 'googlepinyin' output is utf16 string.
        if (index == 0) {
            cand_str.remove(0, im_get_fixed_len());
        }
    } else {
        cand_str = "";
    }

    delete cand_buf;
    return cand_str;
}

// choose a candidate
unsigned pinyin_im::choose(unsigned index)
{
    size_t left;

//    if (im_get_fixed_len()) { // if fixed at least 1, candidate 0(whole line) is not display.
//        index++;
//    }

    left = im_choose(index);

    left = fix_cand_len(left, output_len);
    if (left < 1) {
        return  0; // rearch over!
    } else {
        return  (left - 1);
    }
}

// unchoose a candidate
unsigned pinyin_im::unchoose()
{
    size_t cand = im_cancel_last_choice();

//    if (im_get_fixed_len()) { // if fixed at least 1, candidate 0 is not display.
//        cand--;
//    }

    cand = fix_cand_len(cand, output_len);

    return  (unsigned)cand;
}
