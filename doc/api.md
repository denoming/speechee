# Introduction

J.A.R.V.I.S speaker service API represents reliable C++ API.
This API is based on DBus communication and wrapped by simple C++ class.

# Purpose

The main purpose of J.A.R.V.I.S speaker service API is providing a reliable way to synthesize text into speech.

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