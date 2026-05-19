
# Real-Time Operating System for STM32F103

This project is a preemptive real-time operating system written for the STM32F103 Cortex-M3 microcontroller. The goal of this project was to better understand how RTOS kernels operate at a low level, including scheduling, context switching, synchronization, and timing management on ARM Cortex-M hardware.

The implementation was heavily inspired by the book *Real-Time Operating Systems for ARM® Cortex™-M Microcontrollers* by Jonathan W. Valvano. While the book primarily targets Texas Instruments MSP432 and TM4C microcontrollers, this project adapts many of the same operating system concepts to the STM32F103 platform.

This RTOS was built primarily as a learning project to explore embedded systems design, ARM exception handling, and low-level kernel architecture without relying on existing frameworks such as FreeRTOS or Zephyr.

## Features

- Preemptive priority-based scheduler
- Round-robin scheduling among equal-priority tasks
- Context switching implemented in ARM assembly
- Circular doubly-linked ready queue
- Delta-encoded sleep queue
- FIFO semaphore blocked queues
- Critical sections using PRIMASK interrupt masking
- Idle task using ARM `WFI` low-power instruction
- Timer-driven thread switching
- Thread stack initialization compatible with Cortex-M exception return behavior

## What is a Real-Time Operating System

In the book, Jonathan Valvano defines a real time operating system as "software that manages [memory, I/O, data, and processors], guaranteeing all timing constraints are satisfied.". In other words the rtos is responsible for handling interactions between different components of a computer's architecture assuring that the task(s) at hand will execute with a certain time constraint, granted that you follow the constraints that it presents to the developer. When mentioning an operating system, some globally known names might arise in your head such as Windows, MacOS, Linux. Although also operating systems, they differ from the ones that are used by microcontrollers in embedded systems. My design more closly resembles the design of operating systems such as FreeRTOS and Zephyr, which are targeted towards microcontrollers such as the STM32. The main differenes between a regular and a real time operating system are that regualrs OS are more complex, best effort based, distribute tasks based on fairness, measure distribution based on average bandiwdth, and have more unpredictable behavior. On the other hand, real-time OS is more simple, gaurentees responses, follows strict timing contrisn, measures distrubution of tasks based on minimum and maxiumum limits, and are used on known components. This is a real-time operating system designed for the STM32 based on C. Similar to higher level programming languages the main goal of this operating system was to allow the developer to use threading, meaning ... . This abstracts away the complex hardware interaction to create a more smoother developing experience for developers. 
