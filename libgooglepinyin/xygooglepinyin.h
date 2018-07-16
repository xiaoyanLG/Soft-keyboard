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

#ifndef SYIMPINYIN_H
#define SYIMPINYIN_H

#include <QObject>

class pinyin_im : public QObject
{
    Q_OBJECT
public:
    explicit pinyin_im(QObject *parent = 0);
    ~pinyin_im();

public:
    bool init(const QString &dir = "", int max_spell_len = 64, int max_output_len = 64);
    void deinit();

    void flush_cache();

    unsigned search(const QString &);
    unsigned del_search(unsigned);
    void reset_search();
    unsigned cur_search_pos();
    QString get_candidate(unsigned);

    unsigned choose(unsigned);
    unsigned unchoose();

private:
    bool enable;
    int  spell_len;
    int  output_len;
};

#endif // SYIMPINYIN_H
