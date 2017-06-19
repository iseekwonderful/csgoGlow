//
//  keyboard.h
//  Wall
//
//  Created by Andre Kalisch on 09.11.16.
//  Copyright © 2016 nadp. All rights reserved.
//

#ifndef keyboard_h
#define keyboard_h
#include <Carbon/Carbon.h>

CGEventRef keyBoardCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp) && (type != kCGEventFlagsChanged)) return event;
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    
    if (keycode == (CGKeyCode)kVK_Control || keycode == (CGKeyCode)kVK_RightControl) {
        ctr = !ctr;
    }
    if (ctr) {
        CGEventSetFlags(event, (CGEventFlags)(NX_CONTROLMASK | CGEventGetFlags(event)));
    }
    
    if (keycode == (CGKeyCode)kVK_Shift || keycode == (CGKeyCode)kVK_RightShift) {
        sft = !sft;
    }
    if (sft) {
        CGEventSetFlags(event, (CGEventFlags)(NX_SHIFTMASK | CGEventGetFlags(event)));
    }
    
    if (keycode == (CGKeyCode)kVK_Command || keycode == (CGKeyCode)kVK_RightCommand) {
        cmd = !cmd;
    }
    if (cmd) {
        CGEventSetFlags(event, (CGEventFlags)(NX_COMMANDMASK | CGEventGetFlags(event)));
    }
    
    if (keycode == (CGKeyCode)kVK_Option || keycode == (CGKeyCode)kVK_RightOption) {
        alt = !alt;
    }
    if (alt) {
        CGEventSetFlags(event, (CGEventFlags)(NX_ALTERNATEMASK | CGEventGetFlags(event)));
    }
    
    CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, (int64_t)keycode);
    if (ctr && alt) {
        if (keycode == kVK_ANSI_V) {
            states = !states;
            printf("State Changed! %s\n", states ? "ON" : "OFF");
        }
    }
    
    return event;
}

void keyBoardListen() {
    CFMachPortRef     eventTap;
    CGEventMask     eventMask;
    CFRunLoopSourceRef runLoopSource;
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventFlagsChanged));
    eventTap = CGEventTapCreate((CGEventTapLocation)kCGSessionEventTap, (CGEventTapPlacement)kCGHeadInsertEventTap, (CGEventTapOptions)0, eventMask, keyBoardCallback, NULL);
    if (!eventTap) {
        exit(1);
    }
    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    CFRunLoopRun();
}

#endif /* keyboard_h */
