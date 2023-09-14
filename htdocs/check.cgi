#!/usr/bin/perl
use strict;
use CGI;

# 创建 CGI 对象
my $cgi = CGI->new;

# 设置响应头
print $cgi->header;

# 从标准输入读取数据（来自 C 语言程序）
my $input_data;
while (<STDIN>) {
    $input_data .= $_;
}

# 处理输入数据（这里简单地将输入数据追加到响应中）
print "Content-Type: text/plain\r\n\r\n";
print "Received input data:\n";
print $input_data;

exit;