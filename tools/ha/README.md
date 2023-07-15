
# Integration

* Copy `speechee` folder into `custom_components` folder;
* Restart HA by power button in "Settings -> System -> Hardware";
* Edit `configuration.yaml` file:
```yaml
speechee:
  url: 'http://<ip-address>:<port>'
```
* Restart HA again.

# Using

Using in developer tools:
```yaml
service: speechee.synthesize_text
data:
  text: "Hello World"
  lang: "en-US"
```

Using in automation:
```yaml
  action:
    - condition: state
      entity_id:
        - sensor.speechee_speaker
      state: Online
    - service: speechee.synthesize_text
      data:
        text: "The door is opened"
        lang: "en-US"
```

