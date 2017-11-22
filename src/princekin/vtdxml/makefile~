CC=g++
 
CFLAGS= -c -O3 -Wall -Winline -fomit-frame-pointer -fforce-addr -frerun-cse-after-loop -fexpensive-optimizations -fregmove -frerun-loop-opt -fmerge-all-constants -fno-branch-count-reg -funroll-loops -fpeephole -march=core2 -falign-functions -falign-loops -falign-jumps -freorder-blocks -freorder-functions -fprefetch-loop-arrays --param inline-unit-growth=250 --param max-inline-recursive-depth=2 --param large-function-growth=650

CFLAGS2 = -c -ggdb
LDFLAGS = -O3 -fomit-frame-pointer -fforce-addr -frerun-cse-after-loop -fexpensive-optimizations -fregmove -frerun-loop-opt -march=core2 -lm 
LDFLAGS2  = -ggdb

#all :  vtd-xml RSSReader stats soap benchmark_vtdxml update indexWriter indexReader testNodeRecorder testOverwrite testBookMark  FragmentTest ModifyXML
all :   RSSReader stats soap benchmark_vtdxml update indexWriter indexReader testNodeRecorder testOverwrite testBookMark  FragmentTest 

benchmark_vtdxml: benchmark_vtdxml.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o variableExpr.o xpath_driver.o

FragmentTest: FragmentTest.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o variableExpr.o xpath_driver.o

#ModifyXML: ModifyXML.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o variableExpr.o

erase: erase.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o variableExpr.o xpath_driver.o

soap: soap.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o variableExpr.o xpath_driver.o

stats: stats.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o variableExpr.o xpath_driver.o

RSSReader: RSSReader.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o variableExpr.o xpath_driver.o

update: update.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o variableExpr.o xpath_driver.o

indexWriter: indexWriter.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o  variableExpr.o xpath_driver.o

indexReader: indexReader.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o  variableExpr.o xpath_driver.o
	
testNodeRecorder: testNodeRecorder.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o  textIter.o  variableExpr.o xpath_driver.o

testOverwrite: testOverwrite.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o  vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o  variableExpr.o xpath_driver.o

testBookMark: testBookMark.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o  variableExpr.o xpath_driver.o

vtd-xml: vtd-xml.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o  variableExpr.o xpath_driver.o

	${CC} ${LDFLAGS} -o vtd-xml vtd-xml.o fastIntBuffer.o arrayList.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdNav_L5.o vtdGen.o autoPilot.o XMLChar.o lex.yy.o xpathParser.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o elementFragmentNs.o transcoder.o textIter.o  variableExpr.o xpath_driver.o

clean:
	-rm *.o soap RSSReader vtd-xml stats

benchmark_vtdxml.o: example/benchmark_vtdxml.cpp
	${CC} ${CFLAGS} example/benchmark_vtdxml.cpp

erase.o : example/erase.cpp
	${CC} ${CFLAGS} example/erase.cpp

FragmentTest.o: example/FragmentTest.cpp
	${CC} ${CFLAGS} example/FragmentTest.cpp

#ModifyXML.o: example/ModifyXML.c
#	${CC} ${CFLAGS} example/ModifyXML.c

#vtd-xml.o: example/vtd-xml.cpp
#	${CC} ${CFLAGS} example/vtd-xml.cpp

RSSReader.o : example/RSSReader.cpp
	${CC} ${CFLAGS} example/RSSReader.cpp

update.o : example/update.cpp
	${CC} ${CFLAGS} example/update.cpp

stats.o : example/stats.cpp
	${CC} ${CFLAGS} example/stats.cpp

soap.o	: example/soap.cpp
	${CC} ${CFLAGS} example/soap.cpp

indexWriter.o	: example/indexWriter.cpp
	${CC} ${CFLAGS} example/indexWriter.cpp

indexReader.o	: example/indexReader.cpp 
	${CC} ${CFLAGS} example/indexReader.cpp

testNodeRecorder.o	: example/testNodeRecorder.cpp
	${CC} ${CFLAGS} example/testNodeRecorder.cpp

testOverwrite.o	: example/testOverwrite.cpp
	${CC} ${CFLAGS} example/testOverwrite.cpp

testBookMark.o 	: example/testBookMark.cpp
	${CC} ${CFLAGS} example/testBookMark.cpp

elementFragmentNs.o  : elementFragmentNs.cpp
	${CC} ${CFLAGS} elementFragmentNs.cpp

bookMark.o : bookMark.cpp
	${CC} ${CFLAGS} bookMark.cpp

nodeRecorder.o	: nodeRecorder.cpp
	${CC} ${CFLAGS} nodeRecorder.cpp

indexHandler.o	: indexHandler.cpp
	${CC} ${CFLAGS} indexHandler.cpp

decoder.o : decoder.cpp
	${CC} ${CFLAGS} decoder.cpp

intHash.o : intHash.cpp
	${CC} ${CFLAGS} intHash.cpp

arrayList.o : arrayList.cpp
	${CC} ${CFLAGS} arrayList.cpp
	
fastIntBuffer.o : fastIntBuffer.cpp
	${CC} ${CFLAGS} fastIntBuffer.cpp

fastLongBuffer.o : fastLongBuffer.cpp
	${CC} ${CFLAGS} fastLongBuffer.cpp

contextBuffer.o : contextBuffer.cpp
	${CC} ${CFLAGS} contextBuffer.cpp  

vtdGen.o : vtdGen.cpp
	${CC} ${CFLAGS} vtdGen.cpp
	
vtdNav.o : vtdNav.cpp
	${CC} ${CFLAGS} vtdNav.cpp

vtdNav_L5.o : vtdNav_L5.cpp
	${CC} ${CFLAGS} vtdNav_L5.cpp

autoPilot.o : autoPilot.cpp
	${CC} ${CFLAGS} autoPilot.cpp

XMLModifier.o : XMLModifier.cpp
	${CC} ${CFLAGS} XMLModifier.cpp
	

XMLChar.o : XMLChar.cpp
	${CC} ${CFLAGS} XMLChar.cpp	

unionExpr.o : unionExpr.cpp
	${CC} ${CFLAGS} unionExpr.cpp

filterExpr.o : filterExpr.cpp
	${CC} ${CFLAGS} filterExpr.cpp

literalExpr.o : literalExpr.cpp
	${CC} ${CFLAGS} literalExpr.cpp

numberExpr.o : numberExpr.cpp
	${CC} ${CFLAGS} numberExpr.cpp

unaryExpr.o : unaryExpr.cpp
	${CC} ${CFLAGS} unaryExpr.cpp

locationPathExpr.o : locationPathExpr.cpp
	${CC} ${CFLAGS} locationPathExpr.cpp


pathExpr.o : pathExpr.cpp
	${CC} ${CFLAGS} pathExpr.cpp

lex.yy.o : lex.yy.cc
	${CC} ${CFLAGS} lex.yy.cc

xpathParser.tab.o : xpathParser.tab.cc
	${CC} ${CFLAGS} xpathParser.tab.cc

funcExpr.o : funcExpr.cpp
	${CC} ${CFLAGS} funcExpr.cpp

variableExpr.o : variableExpr.cpp
	${CC} ${CFLAGS} variableExpr.cpp


xpath_driver.o : xpath_driver.cc
	${CC} ${CFLAGS} xpath_driver.cc

transcoder.o : transcoder.cpp
	${CC} ${CFLAGS} transcoder.cpp

textIter.o  : textIter.cpp
	${CC} ${CFLAGS} textIter.cpp
