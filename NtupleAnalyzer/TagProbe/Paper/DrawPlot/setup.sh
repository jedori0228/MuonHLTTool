#!/bin/bash
cd /Users/jskim/Documents/CMS/MuonHLT/MuonHLTTool/NtupleAnalyzer/
source setup.sh
cd -
alias pl='rsync -auv --delete-excluded outputs/* /Users/jskim/CERNBox/www/MuonHLT/Efficiency/'
alias makehtml='make_html_master --To=MuonHLT/Efficiency/'
export PLOT_PATH=`pwd`/outputs/
