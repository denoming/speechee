# Introduction

Speech component represents a main component of speaker service.

This component has two responsibilities:

* synthesizing readable text into human speech
* playing human speech using default audio device

# Purpose

The purpose is synthesizing readable text into human speech and further playing on audio device.

# Use Cases

* Synthesize text or ssml

```plantuml
@startuml

actor SpeakerService as SS
actor Speaker

usecase "Synthesize text" as UC1
usecase "Synthesize ssml" as UC2

SS --> UC1
SS --> UC2

UC1 --> Speaker
UC2 --> Speaker

@enduml
```

# Component Structure

## Software Units

| Name                     | Description                                                                     |
|--------------------------|---------------------------------------------------------------------------------|
| Player                   | The player to play audio in WAV format on default audio device                  |
| PlayerLoop               | The player loop to handle GLib event loop using dedicated thread                |
| Speaker                  | The speaker to synthesize text to speech and play on player within single queue |
| SpeakerService           | The service to implement API using dbus underlying infrastructure               |
| SpeakerSubsystem         | The subsystem to handle lifetime of service software units                      |
| SpeechSynthesizePool     | The synthesize pool to handle synthesizing tasks                                |
| TextSpeechSynthesizeTask | The task to handle synthesizing text to speech                                  |
| SsmlSpeechSynthesizeTask | The task to handle synthesizing ssml to speech                                  |
| TextToSpeechClient       | The client to work with Google TTS online service                               |

## Class Diagrams

* Subsystem

```plantuml
@startuml

abstract class Subsystem

Subsystem <|-- SpeakerSubsystem

SpeakerSubsystem o-- Speaker
SpeakerSubsystem o-- SpeakerService
SpeakerSubsystem o-- SpeechSynthesizePool
SpeakerSubsystem o-- TextToSpeechClient
SpeakerSubsystem o-- Player

@enduml
```

* Service

```plantuml
@startuml

set separator ::

class SpeakerService {
    +start()
    +stop()    
}

abstract SpeakerAdapter {
    +synthesizeText(text, lang)
    +synthesizeSsml(ssml, lang)
}

SpeakerAdapter <|-- SpeakerService

@enduml
```

* speaker

```plantuml
@startuml

interface IPlayer {
    +state(): PlayState
    +initialize(): bool
    +finalize(): void
    +play(audio): bool
    +onStateUpdate(slot): connection
}

interface ISpeechSynthesizePool {
    +synthesizeText(text, lang, callback)
    +synthesizeSsml(text, lang, callback)
}

IPlayer <|-- Player
ISpeechSynthesizePool <|-- SpeechSynthesizePool

Player o-- AudioBufferList
Player o-- PlayerLoop
Player o-- SpeechSynthesizePool

SpeechSynthesizePool o-- TextSpeechSynthesizeTask : 0..*
SpeechSynthesizePool o-- SsmlSpeechSynthesizeTask : 0..*

@enduml
```

## Sequence Diagram

* Synthesize text

```plantuml
@startuml
participant SpeakerService
participant Speaker
participant SpeechSynthesizePool

->> SpeakerService : synthesizeText(text, lang)
activate SpeakerService
SpeakerService ->> Speaker : synthesizeText(text, lang)
deactivate SpeakerService
activate Speaker
Speaker ->> SpeechSynthesizePool : synthesizeText(text, lang, callback)
activate SpeechSynthesizePool
SpeechSynthesizePool -> SpeechSynthesizePool : pushTask(task: TextSpeechSynthesizeTask)
...
Speaker <<-- SpeechSynthesizePool : onSynthesizeDone(id, audio, errorCode)
deactivate SpeechSynthesizePool
Speaker -> Speaker : playNext()
deactivate Speaker

@enduml
```

* Synthesize SSML

```plantuml
@startuml
participant SpeakerService
participant Speaker
participant SpeechSynthesizePool

->> SpeakerService : synthesizeSsml(ssml, lang)
activate SpeakerService
SpeakerService ->> Speaker : synthesizeSsml(ssml, lang)
deactivate SpeakerService
activate Speaker
Speaker ->> SpeechSynthesizePool : synthesizeSsml(ssml, lang, callback)
activate SpeechSynthesizePool
SpeechSynthesizePool -> SpeechSynthesizePool : pushTask(task: SsmlSpeechSynthesizeTask)
...
Speaker <<-- SpeechSynthesizePool : onSynthesizeDone(id, audio, errorCode)
deactivate SpeechSynthesizePool
Speaker -> Speaker : playNext()
deactivate Speaker

@enduml
```

* Handle synthesizing text task

```plantuml
@startuml

-> SpeechSynthesizePool : pushTask(task)
activate SpeechSynthesizePool
...
loop
    SpeechSynthesizePool -> SpeechSynthesizePool : waitTask()
    SpeechSynthesizePool -> SpeechSynthesizePool : nextTask()
    SpeechSynthesizePool -> TextSpeechSynthesizeTask : perform()
    SpeechSynthesizePool -> TextToSpeechClient : synthesizeText(text, lang, error)
    ...
    SpeechSynthesizePool <-- TextToSpeechClient : synthesizeText(text, lang, error)
    SpeechSynthesizePool -> TextSpeechSynthesizeTask : submit()
end
deactivate SpeechSynthesizePool

@enduml
```

