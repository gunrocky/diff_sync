#!/bin/bash

if [ -f ./dist/Release_MUTEX/GNU-Linux/diff_sync ]; then
    cp ./dist/Release_MUTEX/GNU-Linux/diff_sync ./diff_sync_m
fi

if [ -f ./dist/Release_SPIN/GNU-Linux/diff_sync ]; then
    cp ./dist/Release_SPIN/GNU-Linux/diff_sync ./diff_sync_s
fi

if [ -f ./dist/Release_NONE/GNU-Linux/diff_sync ]; then
    cp ./dist/Release_NONE/GNU-Linux/diff_sync ./diff_sync_n
fi
