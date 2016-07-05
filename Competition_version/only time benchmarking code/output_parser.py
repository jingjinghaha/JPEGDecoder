#!/usr/bin/env python

import sys
import re
import argparse
import urllib2
import os.path


parser = argparse.ArgumentParser()
parser.add_argument("infile", type=str, help="Path to input file")
parser.add_argument("-C", "--dbConnect", help="Send benchmarking results to co8 server", action="store_true")
parser.add_argument("-V", "--valid", help="Should be 1 if image valid, 0 otherwise")
parser.add_argument("-G", "--group", help="Group ID")
parser.add_argument("-I", "--image", help="Image ID")
args = parser.parse_args()

BenchDict = {'group'        : 0,
             'category'     : 0,
             'image'        : 0,
             'runningtime'  : 0,
             'core1time'    : 0,
             'core2time'    : 0,
             'core3time'    : 0,
             'core4time'    : 0,
             'binarysize'   : 1e6,
             'ncores'       : 0,
             'correct'      : 0,
             'version'      : 0,
             'comment'      : ''}

Descriptions = {'version'   : 0,
                'category'  : 0,
                'comment'   : ''}

# Corelabel dict
corelabel = {'MB1':'Core 1 did:\n', 'MB2':'Core 2 did:\n', 'MB3':'Core 3 did:\n', 'MB4':'Core 4 did:\n'}
corelist = {}
oldcore = ''

ListDict = {}

#order of the words in loglines
log_corename = 0
log_timestamp = 2
log_codeDec = 4
log_codeHex = 5
#order of words in error code
code_followLines = 0
code_fileName = 1
code_functionName = 2
code_description = 3

# Core frequencies in MHz
F1 = 60.0
F3 = 100.0

def processBenchmarks(group,image,valid):
    BenchDict['group'] = group

    for desc in Descriptions.keys():
        BenchDict[desc] = Descriptions[desc]

    BenchDict['ncores'] = 0
    if BenchDict['core1time'] > 0:
        BenchDict['ncores'] += 1
    if BenchDict['core2time'] > 0:
        BenchDict['ncores'] += 1
    if BenchDict['core3time'] > 0:
        BenchDict['ncores'] += 1
    if BenchDict['core4time'] > 0:
        BenchDict['ncores'] += 1

    BenchDict['runningtime'] = int(max(BenchDict['core3time'], BenchDict['core4time'], BenchDict['core1time']*(F3/F1), BenchDict['core2time']*(F3/F1)))

    if(image > 0):
        BenchDict['image'] = image

    BenchDict['correct'] = valid


def sendBenchmarks():
    url = 'http://co8.ics.ele.tue.nl/addresult?'
    for param in BenchDict.keys():
        url += str(param)
        url += '='
        url += str(BenchDict[param])
        url += '&'

    print url
    a = urllib2.urlopen(url)
    print a.getcode()


def parseCore(corelist):
    followlines = 0
    addBench = 0
    highpart = 0
    for words in corelist:
        # Now identify the output
        value = re.sub('[()]', '', words[log_codeHex])
        # If we have a new error/verbose/timestamp
        if(followlines == 0):
            outcode = '0x' + value
            # Only print something if it is a valid key
            if outcode in ListDict.keys():
                followlines = int(ListDict[outcode][code_followLines])
                desc = ' '.join(ListDict[outcode][code_description:])
                print desc,
                if(followlines == 0):
                    print '\n'
                else:
                    if desc in BenchDict.keys():
                        benchKey = desc
                        addBench = 1
        # Else we are displaying data from the last error/verbose/timestamp
        else:
            followlines -= 1
            if(followlines == 0):
                print str(value)
            else:
                print str(value) + ' ',

            if(addBench == 1):
                if(followlines > 0):
                    highpart = int(value,16) * pow(2,32)
                else:
                    BenchDict[benchKey] = highpart + int(value, 16)
                    addBench = 0
                    highpart = 0



if __name__ == "__main__":
    f = open('BenchmarkList','r')
    for line in f.readlines():
        words = line.split()
        if(len(words) > 1):
            ListDict.update({words[0]:words[1:]})
    f.close()
    if(os.path.isfile('Description')):
        f = open('Description','r')
        for line in f.readlines():
            words = line.split()
            if(len(words) > 1):
                if words[0] in Descriptions.keys():
                    Descriptions[words[0]] = '_'.join(words[1:])
        f.close()


    f = open(args.infile, 'r')

    for line in f.readlines():
        # Process the line
        words = line.split()
        if len(words) > 0:
            if( words[log_corename] in corelabel.keys() ):
                # Add the line to it's corelist
                core = words[0]
                if core not in corelist.keys():
                    corelist.update({core:[]})
                    print "Updated core "+core
                corelist[core].append(words)

    for core in corelist.keys():
        print corelabel[core]
        parseCore(corelist[core])

    f.close()

    if(args.dbConnect):
        processBenchmarks(args.group,args.image, args.valid)
        sendBenchmarks()


