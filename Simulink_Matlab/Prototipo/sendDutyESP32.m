function y = sendDutyESP32(u)

persistent lastDuty

duty = round(u);
duty = max(0, min(1023, duty));

if isempty(lastDuty) || duty ~= lastDuty
    esp = evalin("base", "esp");
    writeline(esp, string(duty));
    fprintf("Duty enviado desde Simulink: %d\n", duty);
    lastDuty = duty;
end

y = duty;

end