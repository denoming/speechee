# Introduction

Client component represents an implementation of speaker service API.

# Purpose

The purpose is handling DBus communication between public API users and speaker component service
and wrapping all this stuff inside shared library with straightforward API.

# Use Cases

```plantuml
@startuml

actor User
actor SpeakerClient as SC

usecase "Synthesize text" as UC1
usecase "Synthesize ssml" as UC2

User --> UC1
User --> UC2

UC1 --> SC
UC2 --> SC

@enduml
```

# Component Structure

## Software Units

| Name          | Description                                       |
|---------------|---------------------------------------------------|
| SpeakerClient | Represents an implementation of public API client |

## Class Diagrams

```plantuml
@startuml

interface ISpeakerClient {
    synthesizeText(text, lang)
    synthesizeSsml(ssml, lang)
}

ISpeakerClient <|.. SpeakerClient

@enduml
```