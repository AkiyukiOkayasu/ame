// Copyright 2020 Akiyuki Okayasu.
//
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)

#ifndef _NXPLIB_NXPLIB_H_
#define _NXPLIB_NXPLIB_H_

/// コピーコンストラクタ、代入演算子禁止
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &) = delete;   \
    void operator=(const TypeName &) = delete;

#endif /* _NXPLIB_NXPLIB_H_ */