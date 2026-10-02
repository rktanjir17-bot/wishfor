# WishFor — A Collaborative Digital Greeting Magazine

A C++ web application built with the Wt framework, where friends and family can
collaboratively create a digital magazine as a surprise gift for someone's
birthday, farewell, or anniversary.

## Features
- Create a magazine for a recipient and occasion
- Contributors add pages (messages, wishes, photo galleries) via a shared link
- Organizer locks the magazine once ready
- Recipient reveals the finished magazine
- Data persisted as JSON files (File I/O)

## OOP Concepts Demonstrated
- Abstraction, Inheritance, Polymorphism (ContentBlock hierarchy)
- Composition (Magazine -> Page -> ContentBlock)
- State Pattern (CollectingState / LockedState)
- Factory Pattern (OccasionFactory)
- Exception Handling (MagazineLockedException)
- Static members, File I/O (JSON persistence)

## Tech Stack
- C++17
- Wt (C++ Web Toolkit)
- nlohmann/json

## How to Run
```bash
g++ app.cpp ContentBlock.cpp Page.cpp Magazine.cpp MagazineState.cpp OccasionFactory.cpp JsonStore.cpp -o wishfor_app -lwt -lwthttp -std=c++17
./wishfor_app --docroot . --http-address 0.0.0.0 --http-port 8081
```
Then open `http://localhost:8081` in a browser.

## Author
Tanjir — CSE-04, Bangladesh Army University of Science and Technology (BAUSTK)
