/* CoreGraphics - CGRemoteOperation.h
   Copyright (c) 2000-2011 Apple Inc.
   All rights reserved. */

#ifndef CGREMOTEOPERATION_H_
#define CGREMOTEOPERATION_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

#include <CoreGraphics/CGDirectDisplay.h>
#include <CoreGraphics/CGError.h>
#include <CoreGraphics/CGGeometry.h>
#include <CoreFoundation/CFMachPort.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

typedef CGError CGEventErr;

#define CGEventNoErr kCGErrorSuccess

/* A type representing the number of buttons being set in a synthetic mouse
   event. */

typedef uint32_t CGButtonCount;

/* A type representing the number of scrolling devices being set in a
   synthetic scrolling event. */

typedef uint32_t CGWheelCount;

/* A type representing a character generated by pressing one or more keys on
   a keyboard. */

typedef uint16_t CGCharCode;

/* A type representing the virtual key codes used in keyboard events. */

typedef uint16_t CGKeyCode;

/* A client-supplied callback function that’s invoked when an area of the
   display is modified or refreshed.

   When an area of the display is modified or refreshed, your callback
   function will be invoked with a count of the number of rectangles in the
   refreshed areas, and a list of the refreshed rectangles. The rectangles
   are in global coordinates.

   To register a screen refresh callback function, call the function
   `CGRegisterScreenRefreshCallback'. Quartz invokes your callback function
   when operations such as drawing, window movement, scrolling, or display
   reconfiguration occur on local displays. When you are finished using a
   callback registration, call `CGUnregisterScreenRefreshCallback' to remove
   it.

   Note that a single rectangle may occupy multiple displays, either by
   overlapping the displays or by residing on coincident displays when
   mirroring is active. You can use the function `CGGetDisplaysWithRect' to
   determine the displays a rectangle occupies. */

typedef void (*CGScreenRefreshCallback)(uint32_t count,
                                        const CGRect *  rects,
                                        void *__nullable userInfo);

/* Register a callback function to be invoked when local displays are
   refreshed or modified.

   The callback function you register is invoked only if your application
   has an active event loop. The callback is invoked in the same thread of
   execution that is processing events within your application. */

CG_EXTERN CGError CGRegisterScreenRefreshCallback(
    CGScreenRefreshCallback  callback, void * __nullable userInfo)
    API_DEPRECATED("No longer supported", macos(10.0,10.8));

/* Remove a previously registered callback function invoked when local
   displays are refreshed or modified.

   Both the callback function and the `userInfo' argument must match the
   registered entry to be removed. */

CG_EXTERN void CGUnregisterScreenRefreshCallback(
    CGScreenRefreshCallback  callback, void * __nullable userInfo)
    API_DEPRECATED("No longer supported", macos(10.0,10.8));

/* Wait for screen refresh operations.

   In some applications it may be preferable to wait for screen refresh data
   synchronously, using this function. You should call this function in a
   thread other than the main event-processing thread.

   As an alternative, Quartz also supports asynchronous notification --- see
   `CGRegisterScreenRefreshCallback'. If refresh callback functions are
   registered, this function should not be used.

   You must deallocate the returned rectangle array with the function
   `CGReleaseScreenRefreshRects'. */

CG_EXTERN CGError CGWaitForScreenRefreshRects(
    CGRect *__nullable * __nullable rects, uint32_t * __nullable count)
    API_DEPRECATED("No longer supported", macos(10.0,10.8));

/* A type for `CGWaitForScreenUpdateRects' specifying the desired types of
   screen update operations. */

typedef CF_OPTIONS(uint32_t, CGScreenUpdateOperation) {
    kCGScreenUpdateOperationRefresh = 0,
    kCGScreenUpdateOperationMove = (1u << 0),
    kCGScreenUpdateOperationReducedDirtyRectangleCount = (1u << 31)
};

/* A type representing the distance a region on the screen moves in pixel
   units.

   The fields `dX' and `dY' describe the direction of movement. Positive
   values of `dX' indicate movement to the right; negative values indicate
   movement to the left. Positive values of `dY' indicate movement downward;
   negative values indicate movement upward. */

struct CGScreenUpdateMoveDelta {
    int32_t dX, dY;
};
typedef struct CGScreenUpdateMoveDelta CGScreenUpdateMoveDelta;

/* A type specifying a client-supplied callback function that’s invoked when
   an area of the display is moved.

   When an area of the display is moved, your callback function will be
   invoked with a count of the number of rectangles in the moved area, and a
   list of the moved rectangles. The rectangles are in global coordinates,
   and describe the area prior to the move operation.

   Your function should not modify, deallocate or free memory pointed to by
   `rectArray'.

   A single rectangle may occupy multiple displays, either by overlapping
   the displays or by residing on coincident displays when mirroring is
   active. Use `CGGetDisplaysWithRect' to determine the displays a rectangle
   occupies. */

typedef void (*CGScreenUpdateMoveCallback)(CGScreenUpdateMoveDelta delta,
  size_t count, const CGRect *  rects, void * __nullable userInfo);

/* Register a callback function to be invoked when an area of the display is
   moved. The callback is invoked on the same thread of execution that is
   processing events within your application. */

CG_EXTERN CGError CGScreenRegisterMoveCallback(
    CGScreenUpdateMoveCallback  callback, void * __nullable userInfo)
    API_DEPRECATED("No longer supported", macos(10.3,10.8));

/* Remove a previously registered screen update callback function. */

CG_EXTERN void CGScreenUnregisterMoveCallback(
    CGScreenUpdateMoveCallback  callback, void * __nullable userInfo)
    API_DEPRECATED("No longer supported", macos(10.3,10.8));

/* Wait for screen update operations.

   The parameter `requestedOperations' specifies the desired types of screen
   update operations. There are several possible choices:

   -- Specify `kCGScreenUpdateOperationRefresh' if you want all move
      operations to be returned as refresh operations.

   -- Specify `(kCGScreenUpdateOperationRefresh|kCGScreenUpdateOperationMove)'
      if you want to distinguish between move and refresh operations.

   -- Add `kCGScreenUpdateOperationReducedDirtyRectangleCount' to the screen
      operations if you want to minimize the number of rectangles returned
      to represent changed areas of the display.

   You should deallocate the returned screen update rects by calling
   `CGReleaseScreenRefreshRects'.

   In some applications it may be preferable to wait for screen update data
   synchronously. You should call this function in a thread other than the
   main event-processing thread.

   As an alternative, Quartz also supports asynchronous notification --- see
   `CGRegisterScreenRefreshCallback' and `CGScreenRegisterMoveCallback'. If
   refresh or move callback functions are registered, this function should
   not be used. */

CG_EXTERN CGError CGWaitForScreenUpdateRects(
    CGScreenUpdateOperation requestedOperations,
    CGScreenUpdateOperation * __nullable currentOperation,
    CGRect * __nullable * __nullable rects, size_t * __nullable rectCount,
    CGScreenUpdateMoveDelta * __nullable delta)
    API_DEPRECATED("No longer supported", macos(10.3,10.8));

/* Deallocate the list of rectangles received from
   `CGWaitForScreenRefreshRects' or `CGWaitForScreenUpdateRects'. */

CG_EXTERN void CGReleaseScreenRefreshRects(CGRect * __nullable rects)
    API_DEPRECATED("No longer supported", macos(10.0,10.8));

/* Return true if the mouse cursor is visible, false otherwise. */

CG_EXTERN boolean_t CGCursorIsVisible(void)
    API_DEPRECATED("No longer supported", macos(10.3,10.9));

/* Return true if the mouse cursor is drawn in frame buffer memory, false
   otherwise. (The cursor could exist in an overlay plane or a similar
   mechanism that puts pixels on-screen without altering frame buffer
   content.) If the cursor is drawn in the frame buffer, it is read back
   along with window data.

   The return value is based on the union of the state of the cursor on all
   displays. If the cursor is drawn in the frame buffer on any display, this
   function returns true. */

CG_EXTERN boolean_t CGCursorIsDrawnInFramebuffer(void)
    API_DEPRECATED("No longer supported", macos(10.3,10.9));

/* Move the mouse cursor to the desired position in global display
   coordinates without generating events. */

CG_EXTERN CGError CGWarpMouseCursorPosition(CGPoint newCursorPosition)
    API_AVAILABLE(macos(10.0));

/* After posting a left mouse down with remote mouse drag suppressing
   hardware mouse move events, after some time with no remote mouse drag
   events a warning is logged to aid in diagnosing "my hardware mouse is
   dead" problems.

   Mouse-down conditions of arbitrary length may be produced deliberately,
   as when scrolling through a lengthly document. */

#define kCGMouseDownEventMaskingDeadSwitchTimeout (60.0)

/* Connect or disconnect the mouse and cursor while an application is in the
   foreground.

   When you call this function to disconnect the cursor and mouse, all
   events received by your application have a constant absolute location but
   contain mouse delta (change in X and Y) data. You may hide the cursor or
   change it into something appropriate for your application. You can
   reposition the cursor by using the function `CGDisplayMoveCursorToPoint'
   or `CGWarpMouseCursorPosition'. */

CG_EXTERN CGError CGAssociateMouseAndMouseCursorPosition(boolean_t connected)
    API_AVAILABLE(macos(10.0));

/* Return a CFMachPort that corresponds to the Mac OS X Window Server's
   server port.

   Use this function to detect if the window server process exits or is not
   running. If this function returns NULL, the window server is not running.
   This code example shows how to register a callback function to detect
   when the window server exits:

     static void handleWindowServerDeath(CFMachPortRef port, void *info) {
       printf("Window Server port death detected!\n");
       CFRelease(port);
       exit(1);
     }
     static void watchForServerDeath() {
       CFMachPortRef port = CGWindowServerCreateServerPort();
       CFMachPortSetInvalidationCallBack(port, handleWindowServerDeath);
     }

   Note that when the window server exits, there may be a few seconds during
   which no window server is running. This function will return NULL until a
   new window server is running.

   Your program must run a CFRunLoop for the port death callback to occur. A
   program which does not use a CFRunLoop may periodically call
   `CFMachPortIsValid' to check whether the port is valid. */

CG_EXTERN CFMachPortRef __nullable CGWindowServerCreateServerPort(void)
   API_AVAILABLE(macos(10.8));

/* This function is obsolete. Use Quartz events and Quartz event sources
   instead. */

CG_EXTERN CGError CGEnableEventStateCombining(boolean_t combineState)
    API_DEPRECATED("No longer supported", macos(10.1,10.6));

/* This function obsolete. Use
   `CGEventSourceSetLocalEventsFilterDuringSuppressionState' instead. */

CG_EXTERN CGError CGInhibitLocalEvents(boolean_t inhibit)
    API_DEPRECATED("No longer supported", macos(10.0,10.6));

/* This function is obsolete. Use `CGEventCreateMouseEvent' instead.

   Synthesize a low-level mouse-button event on the local machine.

   The parameter `mouseCursorPosition' specifies the new coordinates of the
   mouse in global display space.

   Pass true for `updateMouseCursorPosition' if the on-screen cursor should
   be moved to the location specified in the `mouseCursorPosition'
   parameter; otherwise, pass false.
   
   The parameter `buttonCount' specifies the number of mouse buttons, up to
   a maximum of 32.

   Pass true for `mouseButtonDown' to specify that the primary or left mouse
   button is down; otherwise, pass false. The remaining parameters are
   Boolean values that specify whether the remaining mouse buttons are down
   (true) or up (false). The second value, if any, should specify the state
   of the secondary mouse button (right). A third value should specify the
   state of the center button, and the remaining buttons should be in USB
   device order.

   Based on the values entered, the appropriate mouse-down, mouse-up,
   mouse-move, or mouse-drag events are generated, by comparing the new
   state with the current state. */

CG_EXTERN CGError CGPostMouseEvent(CGPoint mouseCursorPosition,
    boolean_t updateMouseCursorPosition, CGButtonCount buttonCount,
    boolean_t mouseButtonDown, ... )
    API_DEPRECATED("No longer supported", macos(10.0,10.6));

/* This function is obsolete. Use `CGEventCreateScrollWheelEvent' instead.

   Synthesize scroll wheel events.

   The parameter `wheelCount' specifies the number of scrolling devices, up
   to a maximum of 3.

   The parameter `wheel1' specifies a value that reflects the movement of
   the primary scrolling device on the mouse. The second and third values,
   if any, reflect the movements of the other scrolling devices on the
   mouse.

   Scrolling movement is represented by small signed integer values,
   typically in a range from -10 to +10. Large values may have unexpected
   results, depending on the application that processes the event. */

CG_EXTERN CGError CGPostScrollWheelEvent(CGWheelCount wheelCount,
    int32_t wheel1, ... )
    API_DEPRECATED("No longer supported", macos(10.0,10.6));

/* This function is obsolete. Use `CGEventCreateKeyboardEvent' instead. */

CG_EXTERN CGError CGPostKeyboardEvent(CGCharCode keyChar, CGKeyCode virtualKey,
    boolean_t keyDown)
    API_DEPRECATED("No longer supported", macos(10.0,10.6));

/* A type specifying masks for classes of low-level events that can be
   filtered during event suppression states. */

typedef CF_OPTIONS(uint32_t, CGEventFilterMask) {
    kCGEventFilterMaskPermitLocalMouseEvents = 0x00000001,
    kCGEventFilterMaskPermitLocalKeyboardEvents = 0x00000002,
    kCGEventFilterMaskPermitSystemDefinedEvents = 0x00000004
};

#define kCGEventFilterMaskPermitAllEvents				      \
  (kCGEventFilterMaskPermitLocalMouseEvents				      \
    | kCGEventFilterMaskPermitLocalKeyboardEvents			  \
    | kCGEventFilterMaskPermitSystemDefinedEvents)

/* A type specifying the event suppression states that can occur after
   posting an event. */

typedef CF_ENUM(uint32_t, CGEventSuppressionState) {
    kCGEventSuppressionStateSuppressionInterval = 0,
    kCGEventSuppressionStateRemoteMouseDrag,
    kCGNumberOfEventSuppressionStates
};

/* This function is obsolete. Use
   `CGEventSourceSetLocalEventsFilterDuringSuppressionState' instead. */

CG_EXTERN CGError CGSetLocalEventsFilterDuringSuppressionState(
    CGEventFilterMask filter, CGEventSuppressionState state)
    API_DEPRECATED("No longer supported", macos(10.1,10.6));

/* This function is obsolete. Use
   `CGEventSourceSetLocalEventsSuppressionInterval' instead. */

CG_EXTERN CGError CGSetLocalEventsSuppressionInterval(CFTimeInterval seconds)
    API_DEPRECATED("No longer supported", macos(10.0,10.6));

/* This function is obsolete. Use `CGWindowServerCreateServerPort'
   instead. */

CG_EXTERN CFMachPortRef __nullable CGWindowServerCFMachPort(void) CF_RETURNS_RETAINED
    API_DEPRECATED("No longer supported", macos(10.1,10.8));

/* Obsolete. Present for backwards compatibility with old header typos. */

#define kCGEventSupressionStateSupressionInterval			      \
  kCGEventSuppressionStateSuppressionInterval
#define kCGEventSupressionStateRemoteMouseDrag				      \
  kCGEventSuppressionStateRemoteMouseDrag
#define kCGNumberOfEventSupressionStates				      \
  kCGNumberOfEventSuppressionStates
#define CGEventSupressionState						      \
  CGEventSuppressionState
#define CGSetLocalEventsFilterDuringSupressionState(filter, state)	      \
  CGSetLocalEventsFilterDuringSuppressionState(filter, state)

/* A type representing the count of items in an array of Quartz rectangles. */

typedef uint32_t CGRectCount;

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGREMOTEOPERATION_H_ */
