let a = 3.14;
var b = 42;
print a, " ", b;
#a = 23;
b = 23;
print a, " ", b;

let c = 3;
let d = (1 + 2);
let e = 3 * (1 + 2);

print c, " ", d, " ", c * d, " ", e;
print "c = ", c, " & d = ", d, " & c * d = ", c * d, " & e = ", e;

let arr = [1, 2, 3];

print "arr = ", arr;

print "mixed arr = ", [1, "Hallo"];

let str = "Hallo, Welt!";

print str;

print "arr[1] = ", arr[1], " & arr[1 + 1] = ", arr[1 + 1];
print "arr[1 + 1] + 1 = ", arr[1 + 1] + 1;

print "4 | 8 = ", 4 | 8;
print "~4 = ", ~4, " & !4 = ", !4, " & (4 & 4) = ", (4 & 4), " & ~~4 = ", ~~4, " & !!4 = ", !!4;

var z = 4 ^ 4;
print "z = ", z;
z = z ^ 4;
print "z = ", z;

var eme = [1, 2, 3];
let ia = eme[1];
eme = [4, 5, 6];

print "ia = ", ia, " : ", eme;

eme[1] = 42;

print "ia = ", ia, " : ", eme;

eme[] = 23;

print "ia = ", ia, " : ", eme;

var abc = [];
abc[] = 'x';
abc[] = 'y';
abc[] = 'z';

print "abc: ", abc[0], ':', abc;

let t1 = true;
let t2 = false;
let t3 = null;

print "tfn: ", t1, ':', t2, ':', t3;
print "tfn: ", t1, " <-> ", !t1, " : ", !t2;

let c1 = 1 && 1;
let c2 = 1 || 1;
let c3 = 1 && 0;
let c4 = 1 || 0;

print "and/or: ", c1, ':', c2, ':', c3, ':', c4;

let c5 = 4 > 5;
let c6 = 4 < 5;
let c7 = 42 == 42;
let c8 = 42 == 23;
let c9 = 42 != 23;
let c10 = 42 != 42;

print "cmp: ", c5, ':', c6, ':', c7, ':', c8, ':', c9, ':', c10;

print (1 && 1 || 0);

print "t&f: ", true && false;
print true && true;
print true || false;