#pragma once

class BaseGameEntity {
public:
    explicit BaseGameEntity(int id) {
        setId(id);
    }

    virtual ~BaseGameEntity() = default;

    virtual void update() = 0;

    virtual bool handleMessage(const Telegram& msg) = 0;

    int getId() const noexcept;

private:
    void setId(int id);

private:
    int id_; // unique entity id
    static int next_valid_id_;
};